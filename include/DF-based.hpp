#include <opencv2/opencv.hpp>

void RGBImage2YCbCrImage(const cv::Mat &rgbImage, cv::Mat &ycbcrImage);
void YCbCrImage2RGBImage(const cv::Mat &ycbcrImage, cv::Mat &rgbImage);

float Distortion(cv::Vec4f &Cb, cv::Vec4f &Cr, cv::Vec4f &Cb_b, cv::Vec4f &Cr_b,
                 float Cb_s, float Cr_s);

cv::Vec3b Subsampling420D(const cv::Mat &b2x2);
cv::Vec3b Subsampling420A(const cv::Mat &b2x2);
cv::Vec3b Subsampling420L(const cv::Mat &b2x2);
cv::Vec3b Subsampling420R(const cv::Mat &b2x2);
cv::Vec3b Subsampling420MPEG_B(const cv::Mat &b2x2);

void SubsampleChroma_PI(const cv::Mat &ycbcrImage, cv::Mat &dest,
                        cv::Vec3b (*subsampling)(const cv::Mat &));
void SubsampleChroma_PD_DM(const cv::Mat &bayerImage, cv::Mat &dest,
                           const cv::Mat &ycbcrImage = cv::Mat());
void SubsampleChroma_PD_CDM(
    const cv::Mat &ycbcrImage, cv::Mat &dest,
    cv::Vec3b (*subsampling)(const cv::Mat &) = nullptr);
void SubsampleChroma_PD_Modified420A_COPY(const cv::Mat &ycbcrImage,
                                          cv::Mat &dest);
void SubsampleChroma_PD_BIDM(const cv::Mat &ycbcrImage, cv::Mat &dest,
                             cv::Vec3b (*subsampling)(const cv::Mat &));
