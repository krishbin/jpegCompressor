/* TODO: read the bitmap file and create a multidimensional array containing all
 * the information about the pixels <26-11-20 krishbin> */
/* NOTE: convert the rgb colorspace to CH, Y , Cb, Cr colorspace <26-11-20
 * krishbin> */
/* TODO: do make sure to downsample the cb and cr pixel data by a factor of 2
 * <03-12-20  krishbin paudel> */
// user defined data definition: seperated by _
// definition: camel case
//
//
//
//  read this :::::::::::::::::::::::::::::::::::::::: only 24 bit image support
//  for now no alpha channel support
#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>

#pragma pack(push, 1)
struct BitmapFileHeader {
  uint16_t fileType{0x4D42};  // a two character string to specify DIB file
  uint32_t fileSize{0};       // the total file size
  uint16_t reserved1{0};      // reserved for additional information
  uint16_t reserved2{0};      // reserved for additional information
  uint32_t pixelDataOffset{
      0};  // offset in bytes from first byte in file to the pixel
};

struct BitmapInfoHeader {
  uint32_t header_size{0};  // size of this header
  int32_t width{0};         // width of the final image
  int32_t height{0};        // height of the final image
  // width and height are not signed because they determine how the pixel shall
  // be displayed
  // the negative value are representative of the 2D plane
  uint16_t planes{
      1};  // no of color planes for target device, this is always one
  uint16_t bitCount{0};     // no of bits per pixel
  uint32_t compression{0};  // type of compression used, 0 is uncompressed
  uint32_t compressedSize{
      0};  // size of final compressed image, 0 for uncompressed
  int32_t xPixelsPerMeter{0};  // horizontal resolution of the target device
  int32_t yPixelsPerMeter{0};  // vertical resolution of the target device
  uint32_t colorsTotal{0};  // No. color indexes in the color table. Use 0 for
  // the max number of colors allowed by bit_count
  uint32_t colorsImportant{0};  // No. of colors used for displaying the
  // bitmap. If 0 all colors are required
};

struct BitmapColorHeader {
  uint32_t redMask{0x00ff0000};         // Bit mask for the red channel
  uint32_t greenMask{0x0000ff00};       // Bit mask for the green channel
  uint32_t blueMask{0x000000ff};        // Bit mask for the blue channel
  uint32_t alphaMask{0xff000000};       // Bit mask for the alpha channel
  uint32_t colorSpaceType{0x73524742};  // Default "sRGB" (0x73524742)
  uint32_t unused[16]{0};               // Unused data for sRGB color space
};
#pragma pack(pop)

class color {};

class YCbCrcolor : public color {
 private:
  uint8_t Y;
  uint8_t Cb;
  uint8_t Cr;

 public:
  YCbCrcolor();
  YCbCrcolor(uint8_t Y, uint8_t Cb, uint8_t Cr);
  void getData() {
    std::cout << int(Y) << "\t" << int(Cb) << "\t" << int(Cr) << std::endl;
  };
};

class RGBcolor : public color {
 private:
  uint8_t red;
  uint8_t green;
  uint8_t blue;

 public:
  RGBcolor();
  RGBcolor(uint8_t, uint8_t, uint8_t);

  operator YCbCrcolor() {
    float _Y = 0.299 * red + 0.587 * green + 0.114 * blue;
    float _Cb = -0.1687 * red - 0.3313 * green + 0.5 * blue + 16;
    float _Cr = 0.5 * red - 0.4187 * green - 0.0813 * blue + 16;
    return YCbCrcolor(uint8_t(_Y), uint8_t(_Cb), uint8_t(_Cr));
  };
  void getData() {
    std::cout << int(red) << "\t" << int(green) << "\t" << int(blue)
              << std::endl;
  };
  friend std::ostream& operator<<(std::ostream& s, RGBcolor const&);
  friend std::istream& operator>>(std::istream& s, RGBcolor&);
};

std::ostream& operator<<(std::ostream& s, RGBcolor const& color) {
  return s << color.red << color.green << color.blue;
};
std::istream& operator>>(std::istream& s, RGBcolor& color) {
  return s >> color.red >> color.green >> color.blue;
};
RGBcolor::RGBcolor() { red = 0, blue = 0, green = 0; };
RGBcolor::RGBcolor(uint8_t red, uint8_t green, uint8_t blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
};

YCbCrcolor::YCbCrcolor() { Y = 0, Cb = 0, Cr = 0; };
YCbCrcolor::YCbCrcolor(uint8_t Y, uint8_t Cb, uint8_t Cr) {
  this->Y = Y;
  this->Cb = Cb;
  this->Cr = Cr;
};

enum colorSpace { RGB, YCbCr };
class bitmap {
 private:
  colorSpace type{RGB};
  BitmapFileHeader bmp_file_header;
  BitmapInfoHeader bmp_info_header;
  BitmapColorHeader bmp_color_header;
  uint32_t _Width;
  uint32_t _Height;
  RGBcolor* _RGBData;
  YCbCrcolor* _YCbCrData;

  void FreeMemory() {
    if (_RGBData) {
      delete[] _RGBData;
      _RGBData = NULL;
    };
    if (_YCbCrData) {
      delete[] _YCbCrData;
      _YCbCrData = NULL;
    };
  };
  void FreeMemory(colorSpace _type) {
    if (_type == RGB) {
      delete[] _RGBData;
      _RGBData = NULL;
    };
    if (type == YCbCr) {
      delete[] _YCbCrData;
      _YCbCrData = NULL;
    };
  };

  void Allocate() {
    if (type == RGB) {
      if (_RGBData != NULL) {
        FreeMemory(RGB);
      };
      _RGBData = new RGBcolor[_Width * _Height];
    } else if (type == YCbCr) {
      if (_YCbCrData != NULL) {
        FreeMemory(YCbCr);
      };
      _YCbCrData = new YCbCrcolor[_Width * _Height];
    }
  };

  void convToYCbCr() {
    type = YCbCr;
    Allocate();
    for (int i = 0; i < _Width * _Height; ++i) {
      _YCbCrData[i] = _RGBData[i];
    };
  };

 public:
  bitmap(const std::string filename) { read(filename); };
  ~bitmap() { FreeMemory(); };

  void read(const std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open())
      throw std::runtime_error(
          "Error1:failed to open file, make sure you have a "
          "valid path and a valid address to the file");

    file.read((char*)&bmp_file_header, sizeof(BitmapFileHeader));

    // file type of bmp is specified
    // here 42 is B and 4D is M so this is a hex representation of it as per the
    // bitmap format BM is refered as magic
    if (bmp_file_header.fileType != 0x4D42)
      throw std::runtime_error("Error2:unrecognized fileformat");

    file.read((char*)&bmp_info_header, sizeof(BitmapInfoHeader));

    _Width = abs(bmp_info_header.width);
    _Height = abs(bmp_info_header.height);

    file.read((char*)&bmp_color_header, sizeof(BitmapColorHeader));
    std::cout<< bmp_color_header.colorSpaceType << std::endl;
    // go directly to the pixel data
    file.seekg(bmp_file_header.pixelDataOffset, std::ios::beg);

    if (bmp_info_header.bitCount == 24) {
      Allocate();
      for (int i = 0; i < _Width * _Height; ++i) {
        file.read((char*)&_RGBData[i], sizeof(_RGBData[i]));
      };
    }
    if (bmp_info_header.bitCount == 32) {
      Allocate();
    };

    convToYCbCr();
    file.close();
  };

  void write(const std::string filename) {
    if (_RGBData == NULL) {
      std::runtime_error("Error3:there is nothing to write");
    };
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    bmp_file_header.pixelDataOffset = sizeof(bmp_file_header) +
                                      sizeof(bmp_info_header) +
                                      sizeof(bmp_color_header);
    bmp_file_header.fileSize = bmp_file_header.pixelDataOffset +
                               static_cast<uint32_t>(_Width * _Height * 3);
    file.write((char*)&bmp_file_header, sizeof(bmp_file_header));
    file.write((char*)&bmp_info_header, sizeof(bmp_info_header));
    file.write((char*)&bmp_color_header, sizeof(bmp_color_header));
    for (int i = 0; i < _Width * _Height; ++i) {
      file.write((char*)&_RGBData[i], sizeof(_RGBData[i]));
    };
  };


  void showData(colorSpace _type) {
    if (_type == RGB && _RGBData != NULL) {
      for (int i = 0; i < _Width * _Height; ++i) {
        _RGBData[i].getData();
      };
    } else if (_type == YCbCr && _YCbCrData != NULL) {
      for (int i = 0; i < _Width * _Height; ++i) {
        _YCbCrData[i].getData();
      };
    }
  };
};
