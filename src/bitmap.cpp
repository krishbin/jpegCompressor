/* NOTE: convert the BGR colorspace to CH, Y , Cb, Cr colorspace <26-11-20
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
#include "Chunk.cpp"

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
  uint32_t colorSpaceType{0x73524742};  // Default "sBGR" (0x73524742)
  uint32_t unused[16]{0};               // Unused data for sBGR color space
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
    friend class bitmap; 
};

class BGRcolor : public color {
  private:
    uint8_t blue;
    uint8_t green;
    uint8_t red;

  public:
    BGRcolor();
    BGRcolor(uint8_t, uint8_t, uint8_t);

    operator YCbCrcolor() {
      float _Y = 0.299 * red + 0.587 * green + 0.114 * blue;
      float _Cb = -0.1687 * red - 0.3313 * green + 0.5 * blue + 16;
      float _Cr = 0.5 * red - 0.4187 * green - 0.0813 * blue + 16;
      return YCbCrcolor(uint8_t(_Y), uint8_t(_Cb), uint8_t(_Cr));
    };
    void getData() {
      std::cout << int(blue) << "\t" << int(green) << "\t" << int(red)
        << std::endl;
    };
    friend std::ostream& operator<<(std::ostream& s, BGRcolor const&);
    friend std::istream& operator>>(std::istream& s, BGRcolor&);
};

std::ostream& operator<<(std::ostream& s, BGRcolor const& color) {
  return s << color.blue << color.green << color.red;
};
std::istream& operator>>(std::istream& s, BGRcolor& color) {
  return s >> color.blue >> color.green >> color.red;
};

BGRcolor::BGRcolor() { red = 0, blue = 0, green = 0; };
BGRcolor::BGRcolor(uint8_t blue, uint8_t green, uint8_t red) {
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

enum colorSpace { BGR, YCbCr };

class bitmap {
  private:
    colorSpace type{BGR};
    int padding;
    int numberOfChunks;
    int numberOfSuperChunks;
    int chunkWidth;
    int chunkHeight;
    int superChunkWidth;
    int superChunkHeight;
    BitmapFileHeader bmp_file_header;
    BitmapInfoHeader bmp_info_header;
    BitmapColorHeader bmp_color_header;
    uint32_t _Width;
    uint32_t _Height;
    SuperChunk* _SChunk;
    Chunk<int>* _YChunk;
    Chunk<int>* _CbChunk;
    Chunk<int>* _DownCbChunk;
    Chunk<int>* _CrChunk;
    Chunk<int>* _DownCrChunk;
    BGRcolor* _BGRData;
    YCbCrcolor* _YCbCrData;

    void FreeMemory() {
      if (_BGRData) {
        delete[] _BGRData;
        _BGRData = NULL;
      };
      if (_YCbCrData) {
        delete[] _YCbCrData;
        _YCbCrData = NULL;
      };
      if (_YChunk) {
        delete[] _YChunk;
        _YChunk = NULL;
      };
      if (_CbChunk) {
        delete[] _CbChunk;
        _CbChunk = NULL;
      };
      if (_CrChunk) {
        delete[] _CrChunk;
        _CrChunk = NULL;
      };
      if (_SChunk) {
        delete[] _SChunk;
        _SChunk = NULL;
      };
    };
    void FreeMemory(colorSpace _type) {
      if (_type == BGR) {
        delete[] _BGRData;
        _BGRData = NULL;
      };
      if (type == YCbCr) {
        delete[] _YCbCrData;
        _YCbCrData = NULL;
      };
    };

    void Allocate() {
      if (type == BGR) {
        if (_BGRData != NULL) {
          FreeMemory(BGR);
        };
        _BGRData = new BGRcolor[_Width * _Height];
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
        _YCbCrData[i] = _BGRData[i];
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

      chunkWidth = ceil((float(_Width)/16))*2;
      chunkHeight = ceil((float(_Height)/16))*2;
      /* superChunkWidth = */ 
      /* superChunkHeight = */ 

      padding = _Width % 4 ? (4 - _Width % 4) : 0;
      numberOfChunks=ceil(float(_Width)/16)*ceil(float(_Height)/16)*4;


      file.read((char*)&bmp_color_header, sizeof(BitmapColorHeader));
      // go directly to the pixel data
      file.seekg(bmp_file_header.pixelDataOffset, std::ios::beg);

      if (bmp_info_header.bitCount == 24) {
        Allocate();
        for (int y = _Height - 1; y >= 0; --y) {
          for (int x = 0; x < _Width; ++x) {
            file.read((char*)&_BGRData[y * (_Width) + x],
                sizeof(_BGRData[y * (_Width) + x]));
          };
          // 3 channel skip for padding
          file.seekg(padding * 3, file.cur);
        };
      };

      // support for alpha channel
      if (bmp_info_header.bitCount == 32) {
        Allocate();
      };

      convToYCbCr();
      toChunk();
      downSample();
      file.close();
    };

    void write(const std::string filename) {
      if (_BGRData == NULL) {
        std::runtime_error("Error3:there is nothing to write");
      };
      std::ofstream file(filename, std::ios::out | std::ios::binary);
      bmp_file_header.pixelDataOffset = sizeof(bmp_file_header) +
        sizeof(bmp_info_header) +
        sizeof(bmp_color_header);
      bmp_file_header.fileSize =
        bmp_file_header.pixelDataOffset +
        static_cast<uint32_t>((_Width + padding) * _Height * 3);
      file.write((char*)&bmp_file_header, sizeof(bmp_file_header));
      file.write((char*)&bmp_info_header, sizeof(bmp_info_header));
      file.write((char*)&bmp_color_header, sizeof(bmp_color_header));
      for (int y = _Height - 1; y >= 0; --y) {
        for (int x = 0; x < _Width; ++x) {
          file.write((char*)&_BGRData[y * _Width + x],
              sizeof(_BGRData[y * _Width + x]));
        };
        if (_Width % 4 != 0) {
          for (int pad = 0; pad < padding*3; ++pad) {
            uint8_t padData=0x00;
            file.write((char*)&padData, sizeof(padData));
          }
        }
      };
    };

    void showData(colorSpace _type) {
      if (_type == BGR && _BGRData != NULL) {
        for (int i = 0; i < _Width * _Height; ++i) {
          _BGRData[i].getData();
        };
      } else if (_type == YCbCr && _YCbCrData != NULL) {
        for (int i = 0; i < _Width * _Height; ++i) {
          _YCbCrData[i].getData();
        };
      }
    };

    void toChunk(){
      Chunk<int> _Chunk;
      _YChunk = new Chunk<int>[numberOfChunks];
      _CbChunk = new Chunk<int>[numberOfChunks];
      _CrChunk = new Chunk<int>[numberOfChunks];
      int Ypixels[8][8];
      int Cbpixels[8][8];
      int Crpixels[8][8];
      int count = 0;
      int h,k;
      for (int y = 0 ; y < (16*ceil(float(_Height)/16)) ; y = y+8) {
        for (int x = 0; x < (16*ceil(float(_Width)/16)); x = x+8) {
          for(int v = 0;v<8;++v){
            for(int u = 0;u<8;++u){
              h=x;
              k=y;
              if((x+u)>=_Width){
                h=x-_Width+u;
              }
              if((y+v)>=_Height){
                k=y-_Height+v;
              }
              Ypixels[u][v] = _YCbCrData[(k+v)*_Width + (h+u)].Y;
              Cbpixels[u][v] = _YCbCrData[(k+v)*_Width + (h+u)].Cb;
              Crpixels[u][v] = _YCbCrData[(k+v)*_Width + (h+u)].Cr;
            }
          }
          _YChunk[count]  = DCTandCompressChunk(Chunk<int>(Ypixels),YquantizationTable);
          _CbChunk[count] = DCTandCompressChunk(Chunk<int>(Cbpixels),CquantizationTable);
          _CrChunk[count] = DCTandCompressChunk(Chunk<int>(Crpixels),CquantizationTable);
          _CbChunk[count].display();
          ++count;
        };
      };
    };

    void downSample(){
      int count = 0;
      int CbPixels[8][8];
      int CrPixels[8][8];
      for (int y = 0 ; y < chunkHeight; y=y+2) {
        for (int x = 0; x < chunkWidth; x=x+2) {
          for(int v = 0;v<8;++v){
             for(int u = 0;u<8;++u){
               CbPixels[u][v] = int((_CbChunk[y*chunkWidth + x].pixelValues[u][v] 
                 + _CbChunk[(y+1)*chunkWidth + x].pixelValues[u][v]
                 + _CbChunk[y*chunkWidth + (x+1)].pixelValues[u][v]
                 + _CbChunk[(y+1)*chunkWidth + (x+1)].pixelValues[u][v])/4);
               CrPixels[u][v] = int((_CrChunk[y*chunkWidth + x].pixelValues[u][v] 
                 + _CrChunk[(y+1)*chunkWidth + x].pixelValues[u][v]
                 + _CrChunk[y*chunkWidth + (x+1)].pixelValues[u][v]
                 + _CrChunk[(y+1)*chunkWidth + (x+1)].pixelValues[u][v])/4);
             }
          }
          _CbChunk[count]=Chunk<int>(CbPixels);
          _CrChunk[count]=Chunk<int>(CrPixels);
          ++count;
        };
      };
    };

    void toSuperChunks(){
      int count;
      _SChunk = new SuperChunk[chunkHeight*chunkWidth/4];
      for (int y = 0 ; y < chunkHeight/2; ++y) {
        for (int x = 0; x < chunkWidth/2; ++x) {
          _SChunk[count].Y[0] = _YChunk[y*2*chunkWidth + x*2];
          _SChunk[count].Y[1] = _YChunk[y*2*chunkWidth + (x*2+1)];
          _SChunk[count].Y[2] = _YChunk[(y*2+1)*chunkWidth + x*2];
          _SChunk[count].Y[3] = _YChunk[(y*2+1)*chunkWidth + (x*2+1)];
          _SChunk[count].Cb = _CbChunk[count];
          _SChunk[count].Cr = _CrChunk[count];
          ++count;
        };
      };
    };

};
