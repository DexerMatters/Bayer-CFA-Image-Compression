#include <DF-based.hpp>
#include <array>
#define SUM4(x)                                                                \
  ([&]() -> float {                                                            \
    float sum = 0;                                                             \
    for (int i = 0; i < 4; i++) {                                              \
      sum += (x);                                                              \
    }                                                                          \
    return sum;                                                                \
  })()

#define Sq(x) (x) * (x)

using namespace cv;

const Vec4f e_a = Vec4f(-0.391, 0, 2.018, -0.391);
const Vec4f e_b = Vec4f(-0.813, 1.596, 0, -0.813);

constexpr float BT[3][3] = {0.257, 0.504, 0.098,  -0.148, -0.291,
                            0.439, 0.439, -0.368, -0.071};

// Evaluations

float Distortion(Vec4f &Cb, Vec4f &Cr, Vec4f &Cb_b, Vec4f &Cr_b, float Cb_s,
                 float Cr_s) {
  return (Sq(SUM4(e_a[i] * (Cb[i] - 9.0 / 16.0 * Cb_s - Cb_b[i]) +
                  e_b[i] * (Cr[i] - 9.0 / 16.0 * Cr_s - Cr_b[i]))));
};

float Distortion_(Vec4f &Cb, Vec4f &Cr, float Cb_s, float Cr_s) {
  return SUM4(Sq(e_a[i] * (Cb[i] - Cb_s) + e_b[i] * (Cr[i] - Cr_s)));
};

std::array<float, 2> OptimalRounding(Vec4f &Cb, Vec4f &Cr,
                                     std::map<float, float> &&cands) {
  auto best =
      std::min_element(cands.begin(), cands.end(), [&](auto &a, auto &b) {
        return Distortion_(Cb, Cr, a.first, a.second) <
               Distortion_(Cb, Cr, b.first, b.second);
      });
  return {best->first, best->second};
}

// Chroma subsampling

// Subsampling 4:2:0(D)
Vec3b Subsampling420D(const Mat &b2x2) { return b2x2.at<Vec3b>(0, 0); }

// Subsampling 4:2:0(A)
Vec3b Subsampling420A(const Mat &b2x2) {
  Vec3b b = b2x2.at<Vec3b>(0, 0);
  Vec3b c = b2x2.at<Vec3b>(0, 1);
  Vec3b d = b2x2.at<Vec3b>(1, 0);
  Vec3b e = b2x2.at<Vec3b>(1, 1);
  Vec3b result = (b / 4 + c / 4 + d / 4 + e / 4);
  return result;
}

// Subsampling 4:2:0(L)
Vec3b Subsampling420L(const Mat &b2x2) {
  Vec3b b = b2x2.at<Vec3b>(0, 0); // top-left
  Vec3b c = b2x2.at<Vec3b>(1, 0); // bottom-left
  return (b + c) / 2;
}

// Subsampling 4:2:0(R)
Vec3b Subsampling420R(const Mat &b2x2) {
  Vec3b b = b2x2.at<Vec3b>(0, 1); // top-right
  Vec3b d = b2x2.at<Vec3b>(1, 1); // bottom-right
  return (b + d) / 2;
}

// Subsampling 4:2:0(MPEG-B)
Vec3b Subsampling420MPEG_B(const Mat &b2x2) {
  float tapFilter_[13] = {2, 0, -4, -3, 5, 19, 26, 19, 5, -3, -4, 0, 2};
  Mat tapFilter(1, 13, CV_32F, tapFilter_);
  // Run the filter on the top-left elem of the 2x2 block
  Vec3b elem = b2x2.at<Vec3b>(0, 0);
  Vec3b result(0, 0, 0);
  // Apply the filter to each channel
  for (int i = 0; i < 3; i++) {
    Mat channel(1, 13, CV_32F);
    for (int j = 0; j < 13; j++) {
      channel.at<float>(0, j) = b2x2.at<Vec3b>(0, j)[i];
    }
    Mat filtered;
    filter2D(channel, filtered, -1, tapFilter, Point(-1, -1), 0,
             BORDER_CONSTANT);
    result[i] = filtered.at<float>(0, 0);
  }
  return result;
}

void SubsampleChroma_PI(const Mat &ycbcrImage, Mat &dest,
                        Vec3b (*subsampling)(const Mat &)) {
  dest = Mat(ycbcrImage.rows / 2, ycbcrImage.cols / 2, CV_8UC3);
  for (int i = 0; i < ycbcrImage.rows - 1; i += 2) {
    for (int j = 0; j < ycbcrImage.cols - 1; j += 2) {
      Mat b2x2 = ycbcrImage(Rect(j, i, 2, 2));
      Vec3b result = subsampling(b2x2);
      dest.at<Vec3b>(i / 2, j / 2) = result;
    }
  }
}

void SubsampleChroma_PD_DM(const Mat &bayerImage, Mat &dest,
                           const Mat &ycbcrImage) {
  // row is the y-coordinate, col is the x-coordinate
  dest = Mat(bayerImage.rows / 2, bayerImage.cols / 2, CV_8UC3);
  for (int i = 0; i < bayerImage.rows - 1; i += 2) {
    for (int j = 0; j < bayerImage.cols - 1; j += 2) {
      uchar R2 = bayerImage.at<uchar>(i, j + 1);
      uchar B3 = bayerImage.at<uchar>(i + 1, j);
      if (ycbcrImage.empty())
        // No luminance information is used...
        dest.at<Vec3b>(i / 2, j / 2) = Vec3b(0, B3, R2);
      else {
        auto Y_s = Subsampling420A(ycbcrImage(Rect(j, i, 2, 2)))[0];
        dest.at<Vec3b>(i / 2, j / 2) = Vec3b(Y_s, B3, R2);
      }
    }
  }
}

void SubsampleChroma_PD_CDM(const Mat &ycbcrImage, Mat &dest,
                            Vec3b (*subsampling)(const Mat &)) {
  dest = Mat(ycbcrImage.rows / 2, ycbcrImage.cols / 2, CV_8UC3);
  for (int i = 0; i < ycbcrImage.rows - 1; i += 2) {
    for (int j = 0; j < ycbcrImage.cols - 1; j += 2) {
      Mat b2x2 = ycbcrImage(Rect(j, i, 2, 2));
      auto Cb = Vec4f(b2x2.at<Vec3b>(0, 0)[1], b2x2.at<Vec3b>(0, 1)[1],
                      b2x2.at<Vec3b>(1, 0)[1], b2x2.at<Vec3b>(1, 1)[1]);
      auto Cr = Vec4f(b2x2.at<Vec3b>(0, 0)[2], b2x2.at<Vec3b>(0, 1)[2],
                      b2x2.at<Vec3b>(1, 0)[2], b2x2.at<Vec3b>(1, 1)[2]);
      auto &a = e_a;
      auto &b = e_b;
      float Cb_s =
          (SUM4(Sq(b[i])) * SUM4(Sq(a[i]) * Cb[i] + a[i] * b[i] * Cr[i]) -
           SUM4(a[i] * b[i]) * SUM4(Sq(b[i]) * Cr[i] + a[i] * b[i] * Cb[i])) /
          (Sq(SUM4(a[i] * b[i])) - SUM4(Sq(a[i])) * SUM4(Sq(b[i])));

      float Cr_s =
          (SUM4(Sq(a[i])) * SUM4(Sq(b[i]) * Cr[i] + a[i] * b[i] * Cb[i]) -
           SUM4(a[i] * b[i]) * SUM4(Sq(a[i]) * Cb[i] + a[i] * b[i] * Cr[i])) /
          (Sq(SUM4(a[i] * b[i])) - SUM4(Sq(a[i])) * SUM4(Sq(b[i])));

      uchar Y_s;
      if (subsampling == nullptr)
        Y_s = Subsampling420A(b2x2)[0];
      else {
        Y_s = subsampling(b2x2)[0];
      }
      dest.at<Vec3b>(i / 2, j / 2) = Vec3b(Y_s, -Cb_s, -Cr_s);
    }
  }
}

void SubsampleChroma_PD_Modified420A_COPY(const Mat &ycbcrImage, Mat &dest) {
  dest = Mat(ycbcrImage.rows / 2, ycbcrImage.cols / 2, CV_8UC3);
  for (int i = 0; i < ycbcrImage.rows - 1; i += 2) {
    for (int j = 0; j < ycbcrImage.cols - 1; j += 2) {
      Vec4f Cb(ycbcrImage.at<Vec3b>(i, j)[1], ycbcrImage.at<Vec3b>(i, j + 1)[1],
               ycbcrImage.at<Vec3b>(i + 1, j)[1],
               ycbcrImage.at<Vec3b>(i + 1, j + 1)[1]);
      Vec4f Cr(ycbcrImage.at<Vec3b>(i, j)[2], ycbcrImage.at<Vec3b>(i, j + 1)[2],
               ycbcrImage.at<Vec3b>(i + 1, j)[2],
               ycbcrImage.at<Vec3b>(i + 1, j + 1)[2]);
      float Cb_s = SUM4(Cb[i]) / 4.0;
      float Cr_s = SUM4(Cr[i]) / 4.0;

      // Find the best candidate by minimizing the distortion
      auto [Cb_best, Cr_best] = OptimalRounding(Cb, Cr,
                                                {{floorf(Cb_s), floorf(Cr_s)},
                                                 {floorf(Cb_s), ceilf(Cr_s)},
                                                 {ceilf(Cb_s), floorf(Cr_s)},
                                                 {ceilf(Cb_s), ceilf(Cr_s)}});
      auto Y_s = Subsampling420A(ycbcrImage(Rect(j, i, 2, 2)))[0];
      dest.at<Vec3b>(i / 2, j / 2) = Vec3b(Y_s, Cb_best, Cr_best);
    }
  }
}

void SubsampleChroma_PD_BIDM(const Mat &ycbcrImage, Mat &dest) {

  // Auxiliar functions
  auto averge = [](float a, float b, float c) {
    constexpr float _16 = 1.0 / 16.0;
    return _16 * a + 3 * _16 * b + 3 * _16 * c;
  };

  auto sub = [](Vec4f &Cr_b, Vec4f &Cb_b, Vec4f &Cr,
                Vec4f &Cb) -> std::array<float, 2> {
    auto &a = e_a;
    auto &b = e_b;

    // Cb_s
    float Cb_s = (SUM4(Sq(b[i])) * SUM4(Sq(a[i]) * (Cb_b[i] - Cb[i]) +
                                        a[i] * b[i] * (Cr_b[i] - Cr[i])) -
                  SUM4(a[i] * b[i]) * SUM4(Sq(b[i]) * (Cr_b[i] - Cr[i]) +
                                           a[i] * b[i] * (Cb_b[i] - Cb[i]))) /
                 (9.0 / 16.0 *
                  (Sq(SUM4(a[i] * b[i])) - SUM4(Sq(a[i])) * SUM4(Sq(b[i]))));

    // Cr_s
    float Cr_s = (SUM4(Sq(a[i])) * SUM4(Sq(b[i]) + (Cr_b[i] - Cr[i]) +
                                        a[i] * b[i] * (Cb_b[i] - Cb[i])) -
                  SUM4(a[i] * b[i]) * SUM4(Sq(a[i]) * (Cb_b[i] - Cb[i]) +
                                           a[i] * b[i] * (Cr_b[i] - Cr[i]))) /
                 (9.0 / 16.0 *
                  (Sq(SUM4(a[i] * b[i])) - SUM4(Sq(a[i])) * SUM4(Sq(b[i]))));

    return {Cb_s, Cr_s};
  };

  // Extend four borders of the YCbCr image by 2 pixels
  Mat image;
  copyMakeBorder(ycbcrImage, image, 2, 2, 2, 2, BORDER_REPLICATE);

  auto mark = Mat_<bool>(image.rows / 2, image.cols / 2);
  dest = Mat(ycbcrImage.rows / 2, ycbcrImage.cols / 2, CV_8UC3);

  // Subsample the chroma channels
  Mat default_s;
  SubsampleChroma_PI(image, default_s, Subsampling420A);

  for (int i = 2; i < image.rows - 2; i += 2) {
    for (int j = 2; j < image.cols - 2; j += 2) {

      // Privot for C_s
      int i_ = i / 2;
      int j_ = j / 2;

      // Privot for the destination image
      int i_dest = i_ - 1;
      int j_dest = j_ - 1;

      // Adjacent blocks
      // +-----+-----+-----+
      // |C_01 |C_11 |C_21 |
      // +-----+-----+-----+
      // |C_00 |C_s  |C_20 |
      // +-----+-----+-----+
      // |C_0n1|C_1n1|C_2n1|
      // +-----+-----+-----+
      auto C_01 = default_s.at<Vec3b>(i_ - 1, j_ - 1);
      auto C_11 = default_s.at<Vec3b>(i_ - 1, j_);
      auto C_21 = default_s.at<Vec3b>(i_ - 1, j_ + 1);
      auto C_00 = default_s.at<Vec3b>(i_, j_ - 1);
      auto C_20 = default_s.at<Vec3b>(i_, j_ + 1);
      auto C_0n1 = default_s.at<Vec3b>(i_ + 1, j_ - 1);
      auto C_1n1 = default_s.at<Vec3b>(i_ + 1, j_);
      auto C_2n1 = default_s.at<Vec3b>(i_ + 1, j_ + 1);

      Vec4f Cb(image.at<Vec3b>(i, j)[1], image.at<Vec3b>(i, j + 1)[1],
               image.at<Vec3b>(i + 1, j)[1], image.at<Vec3b>(i + 1, j + 1)[1]);
      Vec4f Cr(image.at<Vec3b>(i, j)[2], image.at<Vec3b>(i, j + 1)[2],
               image.at<Vec3b>(i + 1, j)[2], image.at<Vec3b>(i + 1, j + 1)[2]);

      Vec4f Cb_b(averge(C_01[1], C_11[1], C_00[1]),
                 averge(C_21[1], C_11[1], C_20[1]),
                 averge(C_0n1[1], C_1n1[1], C_00[1]),
                 averge(C_2n1[1], C_1n1[1], C_20[1]));
      Vec4f Cr_b(averge(C_01[2], C_11[2], C_00[2]),
                 averge(C_21[2], C_11[2], C_20[2]),
                 averge(C_0n1[2], C_1n1[2], C_00[2]),
                 averge(C_2n1[2], C_1n1[2], C_20[2]));
      auto [Cb_s, Cr_s] = sub(Cr_b, Cb_b, Cr, Cb);

      // First step
      if (i == 2 && j == 2)
        dest.at<Vec3b>(i_dest, j_dest) =
            Vec3b(default_s.at<Vec3b>(i_, j_)[0], Cb_s, Cr_s);
      else {
        // clang-format off
        auto C_01_ = mark.at<bool>(i_ - 1, j_ - 1) ? dest.at<Vec3b>(i_dest - 1, j_dest - 1): C_01;
        auto C_11_ = mark.at<bool>(i_ - 1, j_) ? dest.at<Vec3b>(i_dest - 1, j_dest): C_11;
        auto C_21_ = mark.at<bool>(i_ - 1, j_ + 1) ? dest.at<Vec3b>(i_dest - 1, j_dest + 1) : C_21;
        auto C_00_ = mark.at<bool>(i_, j_ - 1) ? dest.at<Vec3b>(i_dest, j_dest - 1) : C_00;
        auto C_20_ = mark.at<bool>(i_, j_ + 1) ? dest.at<Vec3b>(i_dest, j_dest + 1) : C_20;
        auto C_0n1_ = mark.at<bool>(i_ + 1, j_ - 1) ? dest.at<Vec3b>(i_dest + 1, j_dest - 1) : C_0n1;
        auto C_1n1_ = mark.at<bool>(i_ + 1, j_) ? dest.at<Vec3b>(i_dest + 1, j_dest) : C_1n1;
        auto C_2n1_ = mark.at<bool>(i_ + 1, j_ + 1) ? dest.at<Vec3b>(i_dest + 1, j_dest + 1) : C_2n1;
        // clang format on
        Vec4f Cb_b_(averge(C_01_[1], C_11_[1], C_00_[1]),
                    averge(C_21_[1], C_11_[1], C_20_[1]),
                    averge(C_0n1_[1], C_1n1_[1], C_00_[1]),
                    averge(C_2n1_[1], C_1n1_[1], C_20_[1]));
        Vec4f Cr_b_(averge(C_01_[2], C_11_[2], C_00_[2]),
                    averge(C_21_[2], C_11_[2], C_20_[2]),
                    averge(C_0n1_[2], C_1n1_[2], C_00_[2]),
                    averge(C_2n1_[2], C_1n1_[2], C_20_[2]));
        auto [Cb_s_, Cr_s_] = sub(Cr_b_, Cb_b_, Cr, Cb);
        // Compare the distortion
        auto d1 = Distortion(Cb, Cr, Cb_b, Cr_b, Cb_s, Cr_s);
        auto d2 = Distortion(Cb, Cr, Cb_b_, Cr_b_, Cb_s_, Cr_s_);
        float Cb_s_final, Cr_s_final;
        if (d1 < d2) {
          Cb_s_final = Cb_s;
          Cr_s_final = Cr_s;
        } else {
          Cb_s_final = Cb_s_;
          Cr_s_final = Cr_s_;
        }

        auto [Cb_best, Cr_best] = OptimalRounding(Cb, Cr,
                                                  {{floorf(Cb_s_final), floorf(Cr_s_final)},
                                                   {floorf(Cb_s_final), ceilf(Cr_s_final)},
                                                   {ceilf(Cb_s_final), floorf(Cr_s_final)},
                                                   {ceilf(Cb_s_final), ceilf(Cr_s_final)}});


        dest.at<Vec3b>(i_dest, j_dest) =
            Vec3b(default_s.at<Vec3b>(i_, j_)[0], Cb_best, Cr_best);
      }

      // Mark the block as processed
      mark.at<bool>(i_, j_) = true;
    }
  }
}
