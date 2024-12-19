#include <algorithm>
#include <luma.hpp>
#include <map>
#define Sq(x) (x) * (x)
#define NORMALIZE(x) (x > 255 ? 255 : (x < 0 ? 0 : x))

using namespace cv;

float PixelDistortion(int i, const Vec4f &GRBG, float Y, float Cr, float Cb) {
  if (i == 0 || i == 3)
    return Sq(GRBG[i] - 1.164 * (Y - 16) - 0.391 * (Cb - 128) -
              0.813 * (Cr - 128));
  else if (i == 1)
    return Sq(GRBG[i] - 1.164 * (Y - 16) + 1.596 * (Cr - 128));
  else
    return Sq(GRBG[i] - 1.164 * (Y - 16) + 2.018 * (Cb - 128));
}

void LumaModification(const Mat &ycbcrImage, const Mat &bayerImage,
                      Mat &subsampledImage) {
  for (int i = 0; i < ycbcrImage.rows - 1; i += 2) {
    for (int j = 0; j < ycbcrImage.cols - 1; j += 2) {

      float Cr = subsampledImage.at<Vec3b>(i / 2, j / 2)[2];
      float Cb = subsampledImage.at<Vec3b>(i / 2, j / 2)[1];
      Vec4f GRBG(bayerImage.at<uchar>(i, j), bayerImage.at<uchar>(i, j + 1),
                 bayerImage.at<uchar>(i + 1, j),
                 bayerImage.at<uchar>(i + 1, j + 1));

      // clang-format off
      Vec4f Ys((GRBG[0] + 0.391 * (Cb - 128) + 0.813 * (Cr - 128)) / 1.164 + 16,
               (GRBG[1] - 1.596 * (Cr - 128)) / 1.164 + 16,
               (GRBG[2] - 2.018 * (Cb - 128)) / 1.164 + 16,
               (GRBG[3] + 0.391 * (Cb - 128) + 0.813 * (Cr - 128)) / 1.164 + 16);
      auto y = ycbcrImage.at<Vec3b>(i, j)[0];
      // clang-format on

      // Candidate Ys

      std::map<int, float> Ys_ = {
          {0, floorf(Ys[0])}, {0, ceilf(Ys[0])},  {1, floorf(Ys[1])},
          {1, ceilf(Ys[1])},  {2, floorf(Ys[2])}, {2, ceilf(Ys[2])},
          {3, floorf(Ys[3])}, {3, ceilf(Ys[3])},
      };

      auto best = std::min_element(
          Ys_.begin(), Ys_.end(),
          [&](const std::pair<int, float> &a, const std::pair<int, float> &b) {
            return PixelDistortion(a.first, GRBG, a.second, Cr, Cb) <
                   PixelDistortion(b.first, GRBG, b.second, Cr, Cb);
          });
      auto Y_o = subsampledImage.at<Vec3b>(i / 2, j / 2)[0];
      // Update the ycbrImage
      subsampledImage.at<Vec3b>(i / 2, j / 2)[0] = NORMALIZE(best->second);
    }
  }
}