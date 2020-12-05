/* TODO: read the bitmap file and create a multidimensional array containing all
 * the information about the pixels <26-11-20 krishbin> */
/* NOTE: convert the rgb colorspace to CH, Y , Cb, Cr colorspace <26-11-20
 * krishbin> */
/* TODO: do make sure to downsample the cb and cr pixel data by a factor of 2
 * <03-12-20  krishbin paudel> */
// user defined data definition: seperated by _
// definition: camel case
#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>

typedef uint32_t UINT;

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

class color {};

class YCbCrcolor : public color {
 private:
  uint8_t Y;
  uint8_t Cb;
  uint8_t Cr;

 public:
  YCbCrcolor();
  YCbCrcolor(uint8_t Y, uint8_t Cb, uint8_t Cr);
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
    std::cout << int(red) << "\t" << int(green) << "\t" << int(blue);
  };
  friend std::ostream& operator<<(std::ostream& s, RGBcolor const&);
  friend std::istream& operator>>(std::istream& s, RGBcolor&);
};

std::ostream& operator<<(std::ostream& s, RGBcolor const& color){
  return s << color.red << color.green << color.blue;
};
std::istream& operator>>(std::istream& s, RGBcolor& color){
  return s >> color.red >> color.green >> color.blue;
};
RGBcolor::RGBcolor() { red = 0, blue = 0, green = 0; };
RGBcolor::RGBcolor(uint8_t red, uint8_t green, uint8_t blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
};

YCbCrcolor::YCbCrcolor(){};
YCbCrcolor::YCbCrcolor(uint8_t Y, uint8_t Cb, uint8_t Cr) {
  this->Y = Y;
  this->Cb = Cb;
  this->Cr = Cr;
};

class bitmap {
 private:
  BitmapFileHeader bmp_file_header;
  BitmapInfoHeader bmp_info_header;
  BitmapColorHeader bmp_color_header;
  UINT _Width;
  UINT _Height;
  RGBcolor* _Data;

 public:
  bitmap(const std::string filename) { read(filename); };
  ~bitmap() { FreeMemory(); };

  void read(const std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open())
      throw std::runtime_error(
          "failed to open file, make sure you have a "
          "valid path and a valid address to the file");

    // read the header file
    file.read((char*)&bmp_file_header, sizeof(BitmapFileHeader));

    // file type of bmp is specified
    // here 42 is B and 4D is M so this is a hex representation of it as per the
    // bitmap format
    if (bmp_file_header.fileType != 0x4D42)
      throw std::runtime_error("unrecognized fileformat");

    file.read((char*)&bmp_info_header, sizeof(BitmapInfoHeader));

    _Width = abs(bmp_info_header.width);
    _Height = abs(bmp_info_header.height);

    // go directly to the pixel data
    file.seekg(bmp_file_header.pixelDataOffset, std::ios::beg);

    RGBcolor c1;
    file.read((char *)&c1,sizeof(c1));
    c1.getData();
    std::cout << std::endl;
    std::cout << (bmp_info_header.bitCount);

    if (bmp_info_header.bitCount == 24) {
      std::cout << "inthe if ";
    }
    if (bmp_info_header.bitCount == 32) {
      Allocate(_Width, _Height);
    };

    file.close();
  };

  void Allocate(UINT Width, UINT Height) {
    if (_Data != NULL) {
      FreeMemory();
    };
    std::cout << int(_Width) << int(_Height) << std::endl;
    _Data = new RGBcolor[Width * Height];
  };

  void FreeMemory() {
    if (_Data) {
      delete[] _Data;
      _Data = NULL;
    };
  };
};
