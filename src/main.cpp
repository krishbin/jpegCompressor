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
  cout << endl << "Which image to compress? " << endl;
  string userInput;
  cin >> userInput;
  //string file_path = "E:/JPEG_TESTS/sample.bmp";
  string file_path = "E:/JPEG_TESTS/" + userInput + ".bmp";
  bitmap file(file_path);
  jfifEncoder jpeg;
  int Width = file.getWidth();
  int Height = file.getHeight();
  jpeg.writeJPGFile("E:/JPEG_TESTS/" + userInput,YquantizationTable,CquantizationTable,Width,Height,jpeg.dummyYDCTable,jpeg.dummyCDCTable,jpeg.dummyYACTable,jpeg.dummyCACTable,file.getSChunk(), file.getNumOfSuperChunks(), file.getBGRInfo());
  return 0;
}
