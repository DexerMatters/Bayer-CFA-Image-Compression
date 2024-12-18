#pragma once
#include <opencv2/opencv.hpp>

void LumaModification(const cv::Mat &ycbcrImage, const cv::Mat &bayerImage,
                      cv::Mat &subsampledImage);