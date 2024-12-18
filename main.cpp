#include <DF-based.hpp>
#include <demosaicing.hpp>
#include <img.hpp>
#include <iostream>
#include <libraw/libraw.h>
#include <luma.hpp>
#include <opencv2/quality.hpp>
#include <string>

#define ROOT std::string("/home/dexer/Repos/cpp/bic/sample/")
#define RESULT_ROOT std::string("/home/dexer/Repos/cpp/bic/sample/result/")

int main(int, char **) {
  int width;
  int height;
  LibRaw rawProcessor;

  cv::Mat i_demo_rgb;
  cv::Mat i_bayer = DumpRAW(rawProcessor, i_demo_rgb, ROOT + "raws/SAMPLE1.ARW",
                            width, height);
  cv::Mat i_YCbCr;
  cv::Mat i_YCbCr_subsampled;
  cv::Mat i_s_rgb;

  cv::cvtColor(i_demo_rgb, i_YCbCr, cv::COLOR_BGR2YCrCb);
  SubsampleChroma_PD_Modified420A_COPY(i_YCbCr, i_YCbCr_subsampled);
  LumaModification(i_YCbCr, i_bayer, i_YCbCr_subsampled);
  cv::cvtColor(i_YCbCr_subsampled, i_s_rgb, cv::COLOR_YCrCb2BGR);
  cv::imshow("i_demo_rgb", i_s_rgb);
  cv::waitKey(0);
  // cv::cvtColor(i_demo_rgb, i_YCbCr, cv::COLOR_BGR2YCrCb);
  return 0;
}
