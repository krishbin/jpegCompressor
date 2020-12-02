#include<iostream>
#include<string>
#include<math.h>
#include<charconv>
#include<fstream>
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
	Chunk(const T values[8][8]) {
		pixelValues = values;
	}

	T getNthValue(int n) {
		if (n >= 64) {
			n = 64;
		}
		if (n < 0) {
			n = 0;
		}
		return pixelValues[(int)(n/8)][8 % n];
	}

	void setNthValue(int n, T value) {
		if (n >= 64) {
			n = 64;
		}
		if (n < 0) {
			n = 0;
		}
		pixelValues[(int)(n / 8)][8 % n] = value;
	}

	Chunk() {
		
	}

	~Chunk() {
	}
};


class HuffmanTable {
public:
	bool AC = true;
	int length;
	int CodeCounts[16];
	string CodeValues;

	HuffmanTable(bool _AC, int _length, int ccounts[16], string cvalues) {
		length = _length;
		AC = _AC;
		for(int i = 0; i < 16; i++)
			CodeCounts[i] = ccounts[i];

		CodeValues = cvalues;
	}
};

static class ChunkTransformer {
private:
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

	int entropyEncodingTable[8][8] = {
		{0,1,5,6,14,15,27,28},
		{2,4,7,13,16,26,29,42},
		{3,8,12,17,25,30,41,43},
		{9,11,18,24,31,40,44,53},
		{10,19,23,32,39,45,52,54},
		{20,22,33,38,46,51,55,60},
		{21,34,37,47,50,56,59,61},
		{35,36,48,49,57,58,62,63}
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
	Chunk<int> YquantizationTable = Chunk<int>(YquantizationTableValues50);
	Chunk<int> CquantizationTable = Chunk<int>(CquantizationTableValues50);

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
				result[k1][k2] = (int)round(DCTresult[k1][k2] / (weights.pixelValues[k1][k2] * 1.0f)) + 128;
				
				if (result[k1][k2] > 255) {
					result[k1][k2] = 255;
				}
				if (result[k1][k2] < 0) {
					result[k1][k2] = 0;
				}
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
				
				if (iresult[k1][k2] > 255) {
					iresult[k1][k2] = 255;
				}
				if (iresult[k1][k2] < 0) {
					iresult[k1][k2] = 0;
				}
			}
		}
		return Chunk<int>(iresult);
	}

	Chunk<char> ConvertToZigZagString(Chunk<int> compressed) {
		int intString[64];
		Chunk<char> result = Chunk<char>();

		for (int i1 = 0; i1 < 8; i1++) {
			for (int i2 = 0; i2 < 8; i2++) {
				intString[entropyEncodingTable[i1][i2]] = compressed.pixelValues[i1][i2];
			}
		}

		for (int i = 0; i < 64; i++) {
			result.setNthValue(i, intString[i]);
		}

		//Translate this to a string somehow
		return result;
	}


};

ChunkTransformer chunkTransformer;


static class baseFunctions {
	template<class T>
	class PairOf {
	public:
		T values[2];

		T getValue(bool first = true) {
			if (first) {
				return values[0];
			}
			else {
				return values[1]
			}
		}

		T getValue(int index) {
			if (index == 0) {
				return values[0];
			}
			else {
				return values[1]
			}
		}

		T firstItem() {
			return values[0];
		}

		T secondItem() {
			return values[1];
		}

		PairOf(T v1, T v2) {
			values[0] = v1;
			values[1] = v2;
		}

		/*PairOf(PairOf other) {
			values[0] = other.getValue(0);
			values[1] = other.getValue(1);
		}*/

		PairOf() {
			values[0] = 0;
			values[1] = 0;
		}
	};


	void writeJPGFile(string filePathWithoutExtension, 
		Chunk<int> QuantisationWeightsY, 
		Chunk<int> QuantisationWeightsC, 
		int resolution[2],
		HuffmanTable HuffmanTablesDC[2],
		HuffmanTable HuffmanTablesAC[2],
		string entropyData
	) {
		fstream new_file;
		new_file.open(filePathWithoutExtension + ".jpeg", ios::out);
		if (!new_file) {
			cout << "File creation failed (location: writeJPGFile)";
			return;
		}

		string result = "";
		//ff d8 (start of file)
		result += static_cast<char>(0xff) + static_cast<char>(0xd8);

		//ff e0 (Tells the application that this is JFIF format)
		result += combineHexChars({0xff,0xe0});
		result += combineHexChars({0x0,0x10,0x4a,0x46,0x49,0x46,0x0,0x01,0x01,0x01,0x0,0x60,0x0,0x60,0x0,0x0});

		//ff db ... ff db (Quantisation tables)
		/*
			ff db [1 byte of information data (AAAA BBBB)
				AAAA refers to either 0000 or 0001, former defining 1 byte table elements, latter defining 2 bytes
				BBBB refers to table index
			] data stream
		*/
		//Storing Luminance Quantisation table
		result += combineHexChars({ 0xff,0xdb, 0x0,0x43 });
		result += combineHexChars({ 0b00010000 });
		Chunk<char> values = chunkTransformer.ConvertToZigZagString(QuantisationWeightsY);
		for (int i = 0; i < 64; i++) {
			result += values.getNthValue(i);
		}
		//Storing Colour Quantisation table
		result += combineHexChars({ 0xff,0xdb, 0x0, 0x43 });
		result += combineHexChars({ 0b00010001 });
		values = chunkTransformer.ConvertToZigZagString(QuantisationWeightsC);
		for (int i = 0; i < 64; i++) {
			result += values.getNthValue(i);
		}

		//ff c0 (start of baseline frame (the only type implemented))
		/*
			ff c0, length, precision, height, width, num channels, channel info (jfif id, sampling, table)
		*/
		//Header
		result += combineHexChars({ 0xff,0xc0, 0x0, 0x11, 0x8 });
		PairOf<PairOf<char>> res = PairOf<PairOf<char>>(intToChars(resolution[0]), intToChars(resolution[1]));
		result += combineHexChars({res.secondItem(), res.firstItem()});
		result += combineHexChars({0x3});

		//Components
		//Y
		result += combineHexChars({0x1, 0x22, 0x00});
		//Cb
		result += combineHexChars({ 0x2, 0x11, 0x01 });
		//Cr
		result += combineHexChars({ 0x3, 0x11, 0x01 });
		

		//ff c4 (Define huffman tables)
		///Length (2 DCs + 2 AC = 2*(31 + 178) = 418)
		///result += combineHexChars({0xff, 0xc4, 0x01, 0xa2});
		//DC tables
		for (int i = 0; i < 2; i++) {
			//separator, length and specification 0x00 (DC baseline))
			PairOf<char> length = intToChars(HuffmanTablesDC[i].length);
			result += combineHexChars({0xff, 0xc4});
			result += combineHexChars({length.firstItem(), length.secondItem()});
			result += combineHexChars({ 0x00 });
			for(int i2 = 0; i2 < 16; i2 ++)
				result += combineHexChars({HuffmanTablesDC[i].CodeCounts[i2]});
			result += HuffmanTablesDC[i].CodeValues;
		}

		//AC tables
		for (int i = 0; i < 2; i++) {
			//separator, length  and specification 0x10 (AC baseline))
			PairOf<char> length = intToChars(HuffmanTablesAC[i].length);
			result += combineHexChars({ 0xff, 0xc4 });
			result += combineHexChars({ length.firstItem(), length.secondItem() });
			result += combineHexChars({ 0x00 });
			for (int i2 = 0; i2 < 16; i2++)
				result += combineHexChars({ HuffmanTablesAC[i].CodeCounts[i2] });
			result += HuffmanTablesAC[i].CodeValues;
		}

		//ffda start of scan
		result += combineHexChars({ 0xff, 0xda});
		//Length (12), number of components (3), component info (id(1,2,3), [higher bits = DC huffman table, lower bits = AC duffman table]
		// Spectral start and end (0 and 63), successive approximation (0)
		result += combineHexChars({ 0x0, 0x0c, 0x03, 0x01, 0x0, 0x02, 0x11, 0x03, 0x11, 0x0, 0x3f, 0x0 });

		//I'll write the string to the file here because this scan data can become really long at times
		new_file << result;
		result = "";
		new_file << entropyData;



		//ff d9 (end of file)
		result += combineHexChars({ 0xff, 0xd9 });
		new_file << result;
		result = "";
		new_file.close();
	}

	template<class T>
	string combineHexChars(std::initializer_list<T> args)
	{
		string result = "";
		for (auto&& arg : args)
		{
			result+= static_cast<char>(arg);
		}

		return string;
	}

	PairOf<char> intToChars(int item) {
		PairOf<char> result;
		if (item > 65535) item = 65536 % item;
		if (item < 0) item = 0;
		result.values[0] = 16 % (int)(item / 256);
		result.values[1] = 16 % item;
	}
	
	
};



