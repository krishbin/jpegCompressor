//
//  main.cpp
//  jpegCompressor
//
//  Created by Krishbin Paudel on Tue Nov 24 21:48:55 2020
//

#include <fstream>
#include <iostream>

#include "bitmap.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
  std::cout << "JPEG Compressor" << std::endl;
  cout << endl;
  string file_path = "../../bmp_images/sample24bit.bmp";
  bitmap file(file_path);
  RGBcolor c1(1,2,3); 
  return 0;
}
