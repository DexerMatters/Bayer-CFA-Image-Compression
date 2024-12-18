#include <fstream>
#include <img.hpp>
#include <iostream>

cv::Mat DumpRawBMP(const std::string &filename, BmpFileHeader &fileHeader,
                   BmpInfoHeader &infoHeader, BmpColorHeader &colorHeader) {
  std::ifstream file(filename);
  if (!file)
    throw std::runtime_error("Unable to open file");

  // Read the file header
  file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
  if (fileHeader.fileType != 0x4D42) // Check if the file is a BMP File
    throw std::runtime_error("The file is not a BMP file");

  // Read the info header
  file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

  // Read the color header
  if (infoHeader.size >= sizeof(BmpInfoHeader))
    file.read(reinterpret_cast<char *>(&colorHeader), sizeof(colorHeader));

  // Set the offset to the beginning of the pixel data
  file.seekg(fileHeader.offsetData);

  cv::Mat img = cv::Mat::zeros(infoHeader.height, infoHeader.width, CV_8U);
  for (int y = 0; y < infoHeader.height; ++y) {
    for (int x = 0; x < infoHeader.width; ++x) {
      img.at<uchar>(infoHeader.height - 1 - y, x) = file.get();
    }
  }

  return img;
}

cv::Mat DumpBMP(const std::string &filename, BmpFileHeader &fileHeader,
                BmpInfoHeader &infoHeader, BmpColorHeader &colorHeader) {
  std::ifstream file(filename);
  if (!file)
    throw std::runtime_error("Unable to open file");

  // Read the file header
  file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
  if (fileHeader.fileType != 0x4D42) // Check if the file is a BMP File
    throw std::runtime_error("The file is not a BMP file");

  // Read the info header
  file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

  // Read the color header
  if (infoHeader.size >= sizeof(BmpInfoHeader))
    file.read(reinterpret_cast<char *>(&colorHeader), sizeof(colorHeader));

  // Set the offset to the beginning of the pixel data
  file.seekg(fileHeader.offsetData);

  if (infoHeader.bitCount == 24) {
    cv::Mat img = cv::Mat::zeros(infoHeader.height, infoHeader.width, CV_8UC3);
    for (int y = 0; y < infoHeader.height; ++y) {
      for (int x = 0; x < infoHeader.width; ++x) {
        img.at<cv::Vec3b>(infoHeader.height - 1 - y, x)[0] = file.get();
        img.at<cv::Vec3b>(infoHeader.height - 1 - y, x)[1] = file.get();
        img.at<cv::Vec3b>(infoHeader.height - 1 - y, x)[2] = file.get();
      }
      // Add the padding
      for (int p = 0; p < infoHeader.width % 4; ++p) {
        file.get();
      }
    }
    return img;
  } else if (infoHeader.bitCount == 32) {
    cv::Mat img = cv::Mat::zeros(infoHeader.height, infoHeader.width, CV_8UC4);
    for (int y = 0; y < infoHeader.height; ++y) {
      for (int x = 0; x < infoHeader.width; ++x) {
        img.at<cv::Vec4b>(infoHeader.height - 1 - y, x)[0] = file.get();
        img.at<cv::Vec4b>(infoHeader.height - 1 - y, x)[1] = file.get();
        img.at<cv::Vec4b>(infoHeader.height - 1 - y, x)[2] = file.get();
        img.at<cv::Vec4b>(infoHeader.height - 1 - y, x)[3] = file.get();
      }
    }
    return img;
  } else {
    throw std::runtime_error("The file is not a 24 or 32-bit BMP file");
  }

  return cv::Mat();
}

cv::Mat DumpRawBMP(const std::string &filename) {
  BmpFileHeader fileHeader;
  BmpInfoHeader infoHeader;
  BmpColorHeader colorHeader;
  return DumpBMP(filename, fileHeader, infoHeader, colorHeader);
}

void DumpBMPInfo(const std::string &filename, BmpFileHeader &fileHeader,
                 BmpInfoHeader &infoHeader, BmpColorHeader &colorHeader) {
  std::ifstream file(filename);
  if (!file)
    throw std::runtime_error("Unable to open file");

  // Read the file header
  file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));

  // Read the info header
  file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

  // Read the color header
  if (infoHeader.size >= sizeof(BmpInfoHeader))
    file.read(reinterpret_cast<char *>(&colorHeader), sizeof(colorHeader));
}

void SaveBMP(const std::string &filename, const cv::Mat &img,
             const BmpFileHeader &fileHeader, const BmpInfoHeader &infoHeader,
             const BmpColorHeader &colorHeader) {
  std::ofstream file(filename, std::ios::binary);
  if (!file)
    throw std::runtime_error("Unable to open file");

  // Write the file header
  file.write(reinterpret_cast<const char *>(&fileHeader), sizeof(fileHeader));

  // Write the info header
  file.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));

  // Write the color header
  if (infoHeader.size >= sizeof(BmpInfoHeader))
    file.write(reinterpret_cast<const char *>(&colorHeader),
               sizeof(colorHeader));

  // Write the pixel data
  if (infoHeader.bitCount == 24) {
    for (int y = 0; y < infoHeader.height; ++y) {
      for (int x = 0; x < infoHeader.width; ++x) {
        cv::Vec3b pixel = img.at<cv::Vec3b>(infoHeader.height - 1 - y, x);
        file.write(reinterpret_cast<const char *>(pixel.val), 3);
      }
      // Add the padding
      for (int p = 0; p < infoHeader.width % 4; ++p) {
        file.put(0);
      }
    }
  } else if (infoHeader.bitCount == 32) {
    for (int y = 0; y < infoHeader.height; ++y) {
      for (int x = 0; x < infoHeader.width; ++x) {
        cv::Vec4b pixel = img.at<cv::Vec4b>(infoHeader.height - 1 - y, x);
        file.write(reinterpret_cast<const char *>(pixel.val), 4);
      }
    }
  } else {
    throw std::runtime_error("The file is not a 24 or 32-bit BMP file");
  }

  file.close();
}

cv::Mat DumpRAW(LibRaw &rawProcessor, cv::Mat &demoImage,
                const std::string &filename, int &width, int &height) {
  rawProcessor.recycle();
  rawProcessor.open_file(filename.c_str());
  rawProcessor.unpack();
  rawProcessor.dcraw_process();
  auto processed = rawProcessor.dcraw_make_mem_image();
  demoImage =
      cv::Mat(processed->height, processed->width, CV_8UC3, processed->data);
  // demoImage to BGR
  cv::cvtColor(demoImage, demoImage, cv::COLOR_RGB2BGR);
  width = processed->width;
  height = processed->height;

  cv::Mat img = cv::Mat(height, width, CV_8U);
  ushort(*imgData)[4] = rawProcessor.imgdata.image;
  for (int i = 0; i < height - 1; i += 2) {
    for (int j = 0; j < width - 1; j += 2) {
      // GBRG
      img.at<uchar>(i, j) = demoImage.at<cv::Vec3b>(i, j)[1];
      img.at<uchar>(i, j + 1) = demoImage.at<cv::Vec3b>(i, j + 1)[0];
      img.at<uchar>(i + 1, j) = demoImage.at<cv::Vec3b>(i + 1, j)[2];
      img.at<uchar>(i + 1, j + 1) = demoImage.at<cv::Vec3b>(i + 1, j + 1)[1];

      // int colorIndices[4] = {
      //     rawProcessor.COLOR(i, j), rawProcessor.COLOR(i, j + 1),
      //     rawProcessor.COLOR(i + 1, j + 1), rawProcessor.COLOR(i + 1, j)};
      // char cdesc[5] = {rawProcessor.imgdata.idata.cdesc[colorIndices[0]],
      //                  rawProcessor.imgdata.idata.cdesc[colorIndices[1]],
      //                  rawProcessor.imgdata.idata.cdesc[colorIndices[2]],
      //                  rawProcessor.imgdata.idata.cdesc[colorIndices[3]],
      //                  '\0'};
      // ushort C1 = imgData[i * width + j][colorIndices[0]];
      // ushort C2 = imgData[i * width + j + 1][colorIndices[1]];
      // ushort C3 = imgData[(i + 1) * width + j + 1][colorIndices[2]];
      // ushort C4 = imgData[(i + 1) * width + j][colorIndices[3]];

      // Any Pattern to GRBG
      // if (strcmp(cdesc, "RGGB") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C2);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C1);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C4);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C3);
      // } else if (strcmp(cdesc, "BGGR") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C3);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C4);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C1);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C2);
      // } else if (strcmp(cdesc, "GBRG") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C1);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C2);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C3);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C4);
      // } else if (strcmp(cdesc, "GRBG") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C4);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C3);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C2);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C1);
      // } else if (strcmp(cdesc, "RGBG") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C2);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C1);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C3);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C4);
      // } else if (strcmp(cdesc, "GBGR") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C3);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C4);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C2);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C1);
      // } else if (strcmp(cdesc, "BGRG") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C1);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C2);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C4);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C3);
      // } else if (strcmp(cdesc, "GRGB") == 0) {
      //   img.at<uchar>(i, j) = scaleTo8bit(C4);
      //   img.at<uchar>(i, j + 1) = scaleTo8bit(C3);
      //   img.at<uchar>(i + 1, j) = scaleTo8bit(C1);
      //   img.at<uchar>(i + 1, j + 1) = scaleTo8bit(C2);
      // } else {
      //   std::cerr << "Unknown color pattern: " << cdesc << std::endl;
      // }
    }
  }

  return img;
}
