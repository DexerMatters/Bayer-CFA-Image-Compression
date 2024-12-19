#pragma once
#include <array>
#include <cstdint>
#include <libraw/libraw.h>
#include <opencv2/opencv.hpp>

// Color space conversion
void BGR2YCbCr(const cv::Mat &bgrImage, cv::Mat &ycbcrImage);

void YCbCr2BGR(const cv::Mat &ycbcrImage, cv::Mat &bgrImage);

// BMP Processing
struct BmpFileHeader {
  uint16_t fileType{0x4D42}; // File type always BM which is 0x4D42
  uint32_t fileSize{0};      // Size of the file (in bytes)
  uint16_t reserved1{0};     // Reserved, always 0
  uint16_t reserved2{0};     // Reserved, always 0
  uint32_t offsetData{
      0}; // Start position of pixel data (bytes from the beginning of the file)
} __attribute__((packed));

struct BmpInfoHeader {
  uint32_t size{0}; // Size of this header (in bytes)
  int32_t width{0}; // width of bitmap in pixels
  int32_t height{
      0}; // width of bitmap in pixels
          // (if positive, bottom-up, with origin in lower left corner)
          // (if negative, top-down, with origin in upper left corner)
  uint16_t planes{1};   // No. of planes for the target device, this is always 1
  uint16_t bitCount{0}; // No. of bits per pixel
  uint32_t compression{0}; // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY
                           // UNCOMPRESSED BMP images
  uint32_t sizeImage{0};   // 0 - for uncompressed images
  int32_t xPixelsPerMeter{0};
  int32_t yPixelsPerMeter{0};
  uint32_t colorsUsed{0}; // No. color indexes in the color table. Use 0 for the
                          // max number of colors allowed by bit_count
  uint32_t colorsImportant{0}; // No. of colors used for displaying the bitmap.
                               // If 0 all colors are required
} __attribute__((packed));

struct BmpColorHeader {
  uint32_t redMask{0x00ff0000};        // Bit mask for the red channel
  uint32_t greenMask{0x0000ff00};      // Bit mask for the green channel
  uint32_t blueMask{0x000000ff};       // Bit mask for the blue channel
  uint32_t alphaMask{0xff000000};      // Bit mask for the alpha channel
  uint32_t colorSpaceType{0x73524742}; // Default "sRGB" (0x73524742)
  std::array<uint32_t, 16> unused{0};  // Unused data for sRGB color space
} __attribute__((packed));

cv::Mat DumpRawBMP(const std::string &filename, BmpFileHeader &fileHeader,
                   BmpInfoHeader &infoHeader, BmpColorHeader &colorHeader);
cv::Mat DumpRawBMP(const std::string &filename);

cv::Mat DumpBMP(const std::string &filename, BmpFileHeader &fileHeader,
                BmpInfoHeader &infoHeader, BmpColorHeader &colorHeader);

void DumpBMPInfo(const std::string &filename, BmpFileHeader &fileHeader,
                 BmpInfoHeader &infoHeader, BmpColorHeader &colorHeader);

void SaveBMP(const std::string &filename, const cv::Mat &img,
             const BmpFileHeader &fileHeader, const BmpInfoHeader &infoHeader,
             const BmpColorHeader &colorHeader);

cv::Mat DumpRAW(LibRaw &rawProcessor, cv::Mat &demoImage,
                const std::string &filename, int &width, int &height);
