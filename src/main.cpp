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
  string file_path = "../../bmp_images/sample1.bmp";
  bitmap file(file_path);
  jfifEncoder jpeg;
  int Width = file.getWidth();
  int Height = file.getHeight();
  int resolution_w = Width;
  int resolution_h = Height;
  /* HuffmanTable dctable[2] = {jpeg.dummyYDCTable,jpeg.dummyCDCTable}; */
  /* HuffmanTable actable[2] = {jpeg.dummyYACTable,jpeg.dummyCACTable}; */
  /* jpeg.writeJPGFile("test",YquantizationTable,CquantizationTable,resolution,dctable,actable,file.getSChunk()); */
  return 0;
}
