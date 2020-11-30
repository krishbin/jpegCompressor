#include<iostream>
#include<string>
#include<math.h>
#define PI 3.141592653589793
using namespace std;


class Bitmap {
	Channel R, G, B, A, Y, Cb, Cr;
};

class Channel {
	int resolution[2] = {128,128};
	Chunk<int> chunks[2][2];

public:
	Channel(Bitmap bmp, int bmpchannel = 0) {
		//Extract info from bitmap and save it here
	}
};

template<class T>
class Chunk {
	

public:
	T pixelValues[8][8];
	Chunk(T values[8][8]) {
		pixelValues = values;
	}

	Chunk() {
		
	}
};

static class ChunkTransformer {
private:
	int compressionWeights50values[8][8] = {
		{16,12,14,14,18,24,49,72},
		{11,12,13,17,22,35,64,92},
		{10,14,16,22,37,55,78,95},
		{16,19,24,29,56,64,89,98},
		{24,26,40,51,68,81,103,112},
		{40,58,57,87,109,104,121,100},
		{51,60,69,80,103,113,120,103},
		{61,55,56,62,77,92,101,99}
	};

	float alpha(int u) {
		if (u = 0) {
			return 0.7071067812f;
		}
		else {
			return 1;
		}
	}
public:
	const Chunk<int> compressionWeights50 = Chunk<int>(compressionWeights50values);

	Chunk<float> DCT(Chunk<int> pixelValues) {
		float result[8][8] = {
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
						result[k1][k2] += (pixelValues.pixelValues[n1][n2] - 128) * cos(((2 * n1 + 1) * k1 * PI) / 16.0) * cos(((2 * n2 + 1) * k2 * PI) / 16.0);
					}
				}
				result[k1][k2] *= 0.25 * alpha(k1) * alpha(k2);
			}
		}
		
		return Chunk<float>(result);
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
						DCTresult[k1][k2] += (pixelValues.pixelValues[n1][n2] - 128) * cos(((2*n1 + 1) * k1 * PI)/16.0) * cos(((2 * n2 + 1) * k2 * PI) / 16.0);
					}
				}
				DCTresult[k1][k2] *= 0.25 * alpha(k1) * alpha(k2);
				result[k1][k2] = (int)round(DCTresult[k1][k2] / (weights.pixelValues[k1][k2] * 1.0f));
			}
		}

		return Chunk<int>(result);
	}

	Chunk<int> CompressChunk(Chunk<float> DCTresult, Chunk<int> weights) {
		int result[8][8] = {
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5},
		{5,5,5,5,5,5,5,5}
		};
		for (int k1 = 0; k1 < 8; k1++) {
			for (int k2 = 0; k2 < 8; k2++) {
				result[k1][k2] = (int)round(DCTresult.pixelValues[k1][k2] / (weights.pixelValues[k1][k2] * 1.0f));
			}
		}
		return Chunk<int>(result);
	}

	Chunk<int> PixelValue(Chunk<int> compressed, Chunk<int> weights) {
		float result[8][8];
		int iresult[8][8];

		for (int k1 = 0; k1 < 8; k1++) {
			for (int k2 = 0; k2 < 8; k2++) {
				result[k1][k2] = compressed.pixelValues[k1][k2];
				result[k1][k2] = result[k1][k2] * weights.pixelValues[k1][k2];

				for (int n1 = 0; n1 < 8; n1++) {
					for (int n2 = 0; n2 < 8; n2++) {
						result[k1][k2] += alpha(k1) * alpha(k2)*(compressed.pixelValues[n1][n2] - 128) * cos(((2 * n1 + 1) * k1 * PI) / 16.0) * cos(((2 * n2 + 1) * k2 * PI) / 16.0);
					}
				}
				iresult[k1][k2] = (int)round(result[k1][k2] * 0.25) + 128;
				
				if (iresult[k1][k2] > 256) {
					iresult[k1][k2] = 256;
				}
				if (iresult[k1][k2] < 0) {
					iresult[k1][k2] = 0;
				}
			}
		}


		return Chunk<int>(iresult);
	}

	string ConvertToBinString(Chunk<int> compressed) {
		int intString[64];

		//Entropy coding

		for (int i = 0; i < 64; i++) {

		}
		return "abcde";
	}


};



