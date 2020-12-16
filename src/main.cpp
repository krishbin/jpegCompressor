//
//  main.cpp
//  jpegCompressor
//
//  Created by Krishbin Paudel on Tue Nov 24 21:48:55 2020
//

#include <fstream>
#include <iostream>

#include "bitmap.cpp"
#include "jpegEncoder.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
  std::cout << "JPEG Compressor" << std::endl;
  string file_path = "E:/sample1.bmp";
  bitmap file(file_path);
  jfifEncoder jpeg;
  int Width = file.getWidth();
  int Height = file.getHeight();
  jpeg.writeJPGFile("E:/test",YquantizationTable,CquantizationTable,Width,Height,jpeg.dummyYDCTable,jpeg.dummyCDCTable,jpeg.dummyYACTable,jpeg.dummyCACTable,file.getSChunk(), file.getNumOfSuperChunks());
  return 0;
}
