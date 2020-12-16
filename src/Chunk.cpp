#pragma once
#include <math.h>
#include <stdexcept>
#include <iostream>

#define PI 3.141592653589793
class bitmap;
template<class T>
class Chunk {
  public:
    T pixelValues[8][8];
    Chunk();	
    Chunk(T values[8][8]);
    Chunk(const T values[8][8]);
    void display(); 
    int* returnPixels();
    T getNthValue(int);
    T getZigZagNthValue(int);
    void setNthValue(int n, T value);
    ~Chunk();
};

class SuperChunk {
  public:
    Chunk<int> Y[4];
    Chunk<int> Cb;
    Chunk<int> Cr;
};

int YquantizationTableValues50[8][8] = {
  {16,12,14,14,18,24,49,72},
  {11,12,13,17,22,35,64,92},
  {10,14,16,22,37,55,78,95},
  {16,19,24,29,56,64,89,98},
  {24,26,40,51,68,81,103,112},
  {40,58,57,87,109,104,121,100},
  {51,60,69,80,103,113,120,103},
  {61,55,56,62,77,92,101,99}
};

int CquantizationTableValues50[8][8] = {
  {17,18,24,47,99,99,99,99},
  {18,21,26,66,99,99,99,99},
  {24,26,56,99,99,99,99,99},
  {47,66,99,99,99,99,99,99},
  {99,99,99,99,99,99,99,99},
  {99,99,99,99,99,99,99,99},
  {99,99,99,99,99,99,99,99},
  {99,99,99,99,99,99,99,99}
};

float alpha(int u) {
  if (u == 0) {
    return 0.7071067812f;
  }
  else {
    return 1;
  }
}

static constexpr int entropyEncodingTable[8][8] = {
  {0,1,5,6,14,15,27,28},
  {2,4,7,13,16,26,29,42},
  {3,8,12,17,25,30,41,43},
  {9,11,18,24,31,40,44,53},
  {10,19,23,32,39,45,52,54},
  {20,22,33,38,46,51,55,60},
  {21,34,37,47,50,56,59,61},
  {35,36,48,49,57,58,62,63}
};

static constexpr int zigZagIndices[64] =
{1,2,9,17,10,3,4,11,18,25,33,26,19,12,5,6,13,20,27,34,41,49,42,35,28,21,14,7,8,15,22,29,36,43,50,57,58,51,44,37,30,23,16,24,31,38,45,52,59,60,53,46,39,32,40,47,54,61,62,55,48,56,63,64};

Chunk<int> YquantizationTable = Chunk<int>(YquantizationTableValues50);
Chunk<int> CquantizationTable = Chunk<int>(CquantizationTableValues50);

template<class T>
Chunk<T>::Chunk(T values[8][8]) {
  for (int i = 0; i < 8; i++) {
    for (int ii = 0; ii < 8; ii++) {
      pixelValues[i][ii] = values[i][ii];
    }
  }

}

template<class T>
Chunk<T>::Chunk(const T values[8][8]) {
  for (int i = 0; i < 8; i++) {
    for (int ii = 0; ii < 8; ii++) {
      pixelValues[i][ii] = values[i][ii];
    }
  }
}

template<class T>
void Chunk<T>::display() {
  for (int i = 0; i < 8; i++) {
    for (int ii = 0; ii < 8; ii++) {
      std::cout<<pixelValues[i][ii]<<"\t";
    }
    std::cout<<std::endl;
  };
};

template<class T>
int* Chunk<T>::returnPixels(){
  return this->pixelValues;
};

template<class T>
T Chunk<T>::getNthValue(int n) {
  if (n >= 64) {
    n = 64;
  }
  if (n < 0) {
    n = 0;
  }
  return pixelValues[(int)(n / 8)][8 % n];
}

template<class T>
T Chunk<T>::getZigZagNthValue(int n) {
  if (n >= 64) {
    n = 64;
  }
  if (n < 0) {
    n = 0;
  }
  int nn = zigZagIndices[n];
  return pixelValues[(int)(nn / 8)][8 % nn];
}

template<class T>
void Chunk<T>::setNthValue(int n, T value) {

  if (n >= 64) {
    n = 64;
  }
  if (n < 0) {
    n = 0;
  }
  pixelValues[(int)(n / 8)][8 % n] = value;
}

template<class T>
Chunk<T>::Chunk() {
  for (int i = 0; i < 8; i++) {
    for (int ii = 0; ii < 8; ii++) {
      pixelValues[i][ii] = 0;
    }
  }
}

template<class T>
Chunk<T>::~Chunk() {
}

Chunk<int> DCTandCompressChunk(Chunk<int> pixelValues, Chunk<int> weights) {
  float DCTresult[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  };
  int result[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  };
  for (int k1 = 0; k1 < 8; k1++) {
    for (int k2 = 0; k2 < 8; k2++) {
      for (int n1 = 0; n1 < 8; n1++) {
        for (int n2 = 0; n2 < 8; n2++) {
          DCTresult[k1][k2] += (pixelValues.pixelValues[n1][n2] - 128) * cos(((2 * n1 + 1) * k1 * PI) / 16.0) * cos(((2 * n2 + 1) * k2 * PI) / 16.0);
        }
      }
      DCTresult[k1][k2] *= 0.25 * alpha(k1) * alpha(k2);
      result[k1][k2] = (int)round(DCTresult[k1][k2] / (weights.pixelValues[k1][k2] * 1.0f));
    }
  }

  return Chunk<int>(result);
}
