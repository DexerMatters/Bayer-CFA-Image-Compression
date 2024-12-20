#include <DF-based.hpp>
#include <demosaicing.hpp>
#include <img.hpp>
#include <iostream>
#include <libraw/libraw.h>
#include <luma.hpp>
#include <opencv2/quality.hpp>
#include <string>
#include <time.h>

#define ROOT std::string("/home/dexer/Repos/cpp/bic/sample/")
#define RESULT_ROOT std::string("/home/dexer/Repos/cpp/bic/sample/result/")

int testn = 0;

void eval(double time, const cv::Mat &i2, const cv::Mat &i1) {
  cv::OutputArray qualityMap = cv::noArray();
  // Size unification
  cv::Mat resized;
  cv::resize(i2, resized, i1.size());
  auto MSE = cv::quality::QualityMSE::compute(resized, i1, qualityMap);
  auto PSNR = cv::quality::QualityPSNR::compute(resized, i1, qualityMap);
  auto SSIM = cv::quality::QualitySSIM::compute(resized, i1, qualityMap);
  printf(R"(
  TIME: %f
  MSE: %f
  PSNR: %f
  SSIM: %f

)",
         time, (MSE[0] + MSE[1] + MSE[2]) / 3.0,
         (PSNR[0] + PSNR[1] + PSNR[2]) / 3.0,
         (SSIM[0] + SSIM[1] + SSIM[2]) / 3.0);
}

void test(LibRaw &rawProcessor, const std::string &filename,
          const std::string testname) {
  testn++;

  // Read the raw image
  clock_t start = clock();
  std::cout << "Test " << testn << ": " << testname << std::endl;
  int width;
  int height;
  cv::Mat i_demo_rgb;
  cv::Mat i_bayer = DumpRAW(rawProcessor, i_demo_rgb, ROOT + "raws/" + filename,
                            width, height);
  clock_t end = clock();
  std::cout << "Readed in " << (double)(end - start) / CLOCKS_PER_SEC
            << " seconds" << std::endl;

  // Convert the RGB image to YCbCr
  cv::Mat i_YCbCr;
  cv::Mat i_YCbCr_subsampled;
  cv::Mat i_bayer_upsampled;
  cv::Mat i_s_rgb;
  BGR2YCbCr(i_demo_rgb, i_YCbCr);

  // Subsample the chroma
  // std::cout << ">>> Method 1: SubsampleChroma_PI_420D" << std::endl;
  // start = clock();
  // SubsampleChroma_PI(i_YCbCr, i_YCbCr_subsampled, Subsampling420D);
  // end = clock();
  // cv::cvtColor(i_YCbCr_subsampled, i_s_rgb, cv::COLOR_YCrCb2BGR);
  // eval((double)(end - start) / CLOCKS_PER_SEC, i_s_rgb, i_demo_rgb);

  // std::cout << ">>> Method 2: SubsampleChroma_PI_420A" << std::endl;
  // start = clock();
  // SubsampleChroma_PI(i_YCbCr, i_YCbCr_subsampled, Subsampling420A);
  // end = clock();
  // cv::cvtColor(i_YCbCr_subsampled, i_s_rgb, cv::COLOR_YCrCb2BGR);
  // eval((double)(end - start) / CLOCKS_PER_SEC, i_s_rgb, i_demo_rgb);

  // std::cout << ">>> Method 3: SubsampleChroma_PD_Modified420A_COPY"
  //           << std::endl;
  // start = clock();
  // SubsampleChroma_PD_Modified420A_COPY(i_YCbCr, i_YCbCr_subsampled);
  // end = clock();
  // YCbCr2BGR(i_YCbCr_subsampled, i_s_rgb);
  // eval((double)(end - start) / CLOCKS_PER_SEC, i_s_rgb, i_demo_rgb);

  // std::cout << ">>> Method 4: SubsampleChroma_PD_CDM" << std::endl;
  // start = clock();
  // SubsampleChroma_PD_CDM(i_YCbCr, i_YCbCr_subsampled, Subsampling420A);
  // end = clock();
  // YCbCr2BGR(i_YCbCr_subsampled, i_s_rgb);
  // eval((double)(end - start) / CLOCKS_PER_SEC, i_s_rgb, i_demo_rgb);
  // i_YCbCr_subsampled.release();
  // i_s_rgb.release();

  std::cout << ">>> Method 5: SubsampleChroma_PD_BIDM" << std::endl;
  start = clock();
  SubsampleChroma_PD_BIDM(i_YCbCr, i_YCbCr_subsampled);
  // LumaModification(i_YCbCr, i_bayer, i_YCbCr_subsampled);
  end = clock();
  // SubsampleChroma_PD_BIDM(i_YCbCr, i_YCbCr_subsampled);

  YCbCr2BGR(i_YCbCr_subsampled, i_s_rgb);
  EncodeNDecodeJPEG2000(i_s_rgb, i_s_rgb);
  // UpsampledBGR2Bayer(i_s_rgb, i_bayer_upsampled);
  eval((double)(end - start) / CLOCKS_PER_SEC, i_s_rgb, i_demo_rgb);
  cv::imshow("Demo", i_s_rgb);
  cv::waitKey(0);
}

int main(int, char **) {
  LibRaw rawProcessor;
  test(rawProcessor, "SAMPLE3.ARW", "1");
  // cv::cvtColor(i_demo_rgb, i_YCbCr, cv::COLOR_BGR2YCrCb);
  return 0;
}
