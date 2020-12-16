
#include<iostream>
#include"Chunk.cpp"
#include<string>
#include<fstream>
#include"Huffman.h"

using namespace std;


class jfifEncoder {

	
	int dummyYDCTableCodeCounts[16] = { 0x00, 0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	string dummyYDCTableCodeValues = combineHexChars({ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b });


	int dummyYACTableCodeCounts[16] = { 0x00, 0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7d };
	string dummyYACTableCodeValues =
		combineHexChars({
		0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,
		0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,
		0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
		0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
		0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
		0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
		0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,
		0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
		0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
		0xf9,0xfa
		});


	int dummyYquantizationTableValues50[8][8] = {
		{0x03,0x02,0x02,0x03,0x02,0x02,0x03,0x03},
		{0x03,0x03,0x04,0x03,0x03,0x04,0x05,0x08},
		{0x05,0x05,0x04,0x04,0x05,0x0a,0x07,0x07},
		{0x06,0x08,0x0c,0x0a,0x0c,0x0c,0x0b,0x0a},
		{0x0b,0x0b,0x0d,0x0e,0x12,0x10,0x0d,0x0e},
		{0x11,0x0e,0x0b,0x0b,0x10,0x16,0x10,0x11},
		{0x13,0x14,0x15,0x15,0x15,0x0c,0x0f,0x17},
		{0x18,0x16,0x14,0x18,0x12,0x14,0x15,0x14}
	};

	

	//Dummy C huffman tables
	int dummyCDCTableCodeCounts[16] = { 0x00, 0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00 };
	string dummyCDCTableCodeValues = combineHexChars({ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b });


	int dummyCACTableCodeCounts[16] = { 0x00, 0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77 };
	string dummyCACTableCodeValues =
		combineHexChars({
		0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
		0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,
		0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
		0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,
		0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,
		0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
		0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,
		0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,
		0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
		0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
		0xf9,0xfa
		});


	int dummyCquantizationTableValues50[8][8] = {
	{0x03,0x04,0x04,0x05,0x04,0x05,0x09,0x14},
	{0x0d,0x0b,0x0d,0x14,0x14,0x14,0x14,0x14},
	{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},
	{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},
	{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},
	{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},
	{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},
	{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14}
	};




	template<class T>
	class PairOf {
	public:
		T values[2];

		T getValue(bool first = true) {
			if (first) {
				return values[0];
			}
			else {
				return values[1];
			}
		}

		T getValue(int index) {
			if (index == 0) {
				return values[0];
			}
			else {
				return values[1];
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


	void ReverseExtend(int value, int &magnitude, int &bits) {
		if (value >= 0) {
			bits = value;
		}
		else {
			value = -value;
			bits = ~value;
		}
		magnitude = 0;
		while (value != 0) {
			value >>= 1;
			++magnitude;
		}
	}

	void writeToFileByteWise(fstream& fileHolder, string moreBits, string& bitHolder) {
		for (int i = 0; i < moreBits.length(); i++) {
			bitHolder += moreBits.at(i);
			if (bitHolder.length() >= 8) {
				int t_int = 0;
				for (int ii = 0; ii < 8; ii++) {
					t_int <<= 1;
					if (bitHolder.at(ii) == '1') {
						t_int += 1;
					}
				}
				bitHolder = "";
				fileHolder << (char)t_int;
				if (t_int == 0xff) {
					fileHolder << (char)(0x00);
				}
			}
		}
	}

	/*template<class T>
	string combineHexChars(std::initializer_list<T> args)
	{
		string result = "";
		for (auto&& arg : args)
		{
			result+= static_cast<char>(arg);
		}

		return result;
	}*/


	string combineHexChars(std::initializer_list<int> args)
	{
		string result = "";
		for (auto&& arg : args)
		{
			result += static_cast<char>(arg);
		}

		return result;
	}


	PairOf<char> intToChars(int item) {
		PairOf<char> result;
		if (item > 65535) item = 65536 % item;
		if (item < 0) item = 0;
		result.values[0] = 16 % (int)(item / 256);
		result.values[1] = 16 % item;
    return result;
	}

public:

	HuffmanTable dummyYDCTable = HuffmanTable(false, 0x1f, dummyYDCTableCodeCounts, dummyYDCTableCodeValues);
	HuffmanTable dummyCDCTable = HuffmanTable(false, 0x1f, dummyCDCTableCodeCounts, dummyCDCTableCodeValues);
	HuffmanTable dummyYACTable = HuffmanTable(true, 0xb5, dummyYACTableCodeCounts, dummyYACTableCodeValues);
	HuffmanTable dummyCACTable = HuffmanTable(true, 0xb5, dummyCACTableCodeCounts, dummyCACTableCodeValues);

	void writeJPGFile(string filePathWithoutExtension, 
		Chunk<int> QuantisationWeightsY, 
		Chunk<int> QuantisationWeightsC, 
		int resolution[2],
		HuffmanTable HuffmanTablesDC[2],
		HuffmanTable HuffmanTablesAC[2],
		SuperChunk theSuperChunks[]
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
		//Chunk<char> values = chunkTransformer.ConvertToZigZagString(QuantisationWeightsY);
		for (int i = 0; i < 64; i++) {
			result += QuantisationWeightsY.getZigZagNthValue(i);
		}
		//Storing Colour Quantisation table
		result += combineHexChars({ 0xff,0xdb, 0x0, 0x43 });
		result += combineHexChars({ 0b00010001 });
		//values = chunkTransformer.ConvertToZigZagString(QuantisationWeightsC);
		for (int i = 0; i < 64; i++) {
			result += QuantisationWeightsC.getZigZagNthValue(i);
		}

		//ff c0 (start of baseline frame (the only type implemented))
		/*
			ff c0, length, precision, height, width, num channels, channel info (jfif id, sampling, table)
		*/
		//Header
		result += combineHexChars({ 0xff,0xc0, 0x0, 0x11, 0x8 });
		PairOf<PairOf<char>> res = PairOf<PairOf<char>>(intToChars(resolution[0]), intToChars(resolution[1]));
		result += res.secondItem().secondItem();
		result += res.secondItem().firstItem();
		result += res.firstItem().secondItem();
		result += res.firstItem().firstItem();
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

			//separator, length and specification 0x00 (DC baseline))
		PairOf<char> length = intToChars(HuffmanTablesDC[0].length);
		result += combineHexChars({0xff, 0xc4});
		result += combineHexChars({length.firstItem(), length.secondItem()});
		result += combineHexChars({ 0x00 });
		for(int i2 = 0; i2 < 16; i2 ++)
			result += combineHexChars({HuffmanTablesDC[0].CodeCounts[i2]});
		result += HuffmanTablesDC[0].CodeValues;

		length = intToChars(HuffmanTablesDC[1].length);
		result += combineHexChars({ 0xff, 0xc4 });
		result += combineHexChars({ length.firstItem(), length.secondItem() });
		result += combineHexChars({ 0x01 });
		for (int i2 = 0; i2 < 16; i2++)
			result += combineHexChars({ HuffmanTablesDC[1].CodeCounts[i2] });
		result += HuffmanTablesDC[1].CodeValues;
		

		//AC tables

			//separator, length  and specification 0x10 (AC baseline))
		length = intToChars(HuffmanTablesAC[0].length);
		result += combineHexChars({ 0xff, 0xc4 });
		result += combineHexChars({ length.firstItem(), length.secondItem() });
		result += combineHexChars({ 0x10 });
		for (int i2 = 0; i2 < 16; i2++)
			result += combineHexChars({ HuffmanTablesAC[0].CodeCounts[i2] });
		result += HuffmanTablesAC[0].CodeValues;

		length = intToChars(HuffmanTablesAC[1].length);
		result += combineHexChars({ 0xff, 0xc4 });
		result += combineHexChars({ length.firstItem(), length.secondItem() });
		result += combineHexChars({ 0x11 });
		for (int i2 = 0; i2 < 16; i2++)
			result += combineHexChars({ HuffmanTablesAC[1].CodeCounts[i2] });
		result += HuffmanTablesAC[1].CodeValues;
		

		//ffda start of scan
		result += combineHexChars({ 0xff, 0xda});
		//Length (12), number of components (3), component info (id(1,2,3), [higher bits = DC huffman table, lower bits = AC duffman table]
		// Spectral start and end (0 and 63), successive approximation (0)
		result += combineHexChars({ 0x0, 0x0c, 0x03, 0x01, 0x0, 0x02, 0x11, 0x03, 0x11, 0x0, 0x3f, 0x0 });

		//I'll write the string to the file here because this scan data can become really long at times
		new_file << result;
		result = "";
		//new_file << entropyData;

		int LastDCDifference[3] = { 0,0,0 };
		string temp = "";
		string bitholder = "";
		for (int x = 0; x < *(&theSuperChunks + 1) - theSuperChunks; x++)
		{
			SuperChunk aSuperChunk = theSuperChunks[x];
			//Encoding the Y component
			for (int counter = 0; counter < 4; counter++) {
				//DC encoding
				int t_dc_mag;
				int t_dc_bits;
				int difference = aSuperChunk.Y[counter].pixelValues[0][0] - LastDCDifference[0];
				LastDCDifference[0] = aSuperChunk.Y[counter].pixelValues[0][0];
				ReverseExtend(difference, t_dc_mag, t_dc_bits);
				temp = HuffmanTransformer::encode(dummyYDCTable, t_dc_mag);
				writeToFileByteWise(new_file, temp, bitholder);
				//WriteRawBits(encodedMagnitude, bits)

				//AC encoding
				int zeros = 0;
				int i = 1;
				while (i < 64) {
					if (aSuperChunk.Y[counter].getZigZagNthValue(i) != 0) {

						while (zeros >= 16) {
							temp = HuffmanTransformer::encode(dummyYACTable, 0xf0);
							writeToFileByteWise(new_file, temp, bitholder);
							zeros = zeros - 16;
						}
						int t_ac_mag;
						int t_ac_bits;
						ReverseExtend(aSuperChunk.Y[counter].getZigZagNthValue(i), t_ac_mag, t_ac_bits);
						temp = HuffmanTransformer::encode(dummyYACTable, zeros << 4 | t_ac_mag);
						writeToFileByteWise(new_file, temp, bitholder);
						//WriteRawBits(encoded, bits)
					}
					else {
						zeros += 1;
					}
					i++;
				}
				if (zeros != 0) {
					temp = HuffmanTransformer::encode(dummyYACTable, zeros << 4 | 0x00);
					writeToFileByteWise(new_file, temp, bitholder);
				}
			}

			//Encoding the Cb component
			int t_dc_mag = 0;
			int t_dc_bits = 0;
			int difference = aSuperChunk.Cb.pixelValues[0][0] - LastDCDifference[0];
			LastDCDifference[1] = aSuperChunk.Cb.pixelValues[0][0];
			ReverseExtend(difference, t_dc_mag, t_dc_bits);
			temp = HuffmanTransformer::encode(dummyCDCTable, t_dc_mag);
			writeToFileByteWise(new_file, temp, bitholder);
			//WriteRawBits(encodedMagnitude, bits)

			//AC encoding
			int zeros = 0;
			int i = 1;
			while (i < 64) {
				if (aSuperChunk.Cb.getZigZagNthValue(i) != 0) {

					while (zeros >= 16) {
						temp = HuffmanTransformer::encode(dummyCACTable, 0xf0);
						writeToFileByteWise(new_file, temp, bitholder);
						zeros = zeros - 16;
					}
					int t_ac_mag;
					int t_ac_bits;
					ReverseExtend(aSuperChunk.Cb.getZigZagNthValue(i), t_ac_mag, t_ac_bits);
					temp = HuffmanTransformer::encode(dummyCACTable, zeros << 4 | t_ac_mag);
					writeToFileByteWise(new_file, temp, bitholder);
					//WriteRawBits(encoded, bits)
				}
				else {
					zeros += 1;
				}
				i++;
			}
			if (zeros != 0) {
				temp = HuffmanTransformer::encode(dummyCACTable, zeros << 4 | 0x00);
				writeToFileByteWise(new_file, temp, bitholder);
			}

			//Encoding the Cr component
			t_dc_mag = 0;
			t_dc_bits = 0;
			difference = aSuperChunk.Cr.pixelValues[0][0] - LastDCDifference[0];
			LastDCDifference[2] = aSuperChunk.Cr.pixelValues[0][0];
			ReverseExtend(difference, t_dc_mag, t_dc_bits);
			temp = HuffmanTransformer::encode(dummyCDCTable, t_dc_mag);
			writeToFileByteWise(new_file, temp, bitholder);
			//WriteRawBits(encodedMagnitude, bits)

			//AC encoding
			zeros = 0;
			i = 1;
			while (i < 64) {
				if (aSuperChunk.Cr.getZigZagNthValue(i) != 0) {

					while (zeros >= 16) {
						temp = HuffmanTransformer::encode(dummyCACTable, 0xf0);
						writeToFileByteWise(new_file, temp, bitholder);
						zeros = zeros - 16;
					}
					int t_ac_mag;
					int t_ac_bits;
					ReverseExtend(aSuperChunk.Cr.getZigZagNthValue(i), t_ac_mag, t_ac_bits);
					temp = HuffmanTransformer::encode(dummyCACTable, zeros << 4 | t_ac_mag);
					writeToFileByteWise(new_file, temp, bitholder);
					//WriteRawBits(encoded, bits)
				}
				else {
					zeros += 1;
				}
				i++;
			}
			if (zeros != 0) {
				temp = HuffmanTransformer::encode(dummyCACTable, zeros << 4 | 0x00);
				writeToFileByteWise(new_file, temp, bitholder);
			}
		}



		//ff d9 (end of file)
		result += combineHexChars({ 0xff, 0xd9 });
		new_file << result;
		result = "";
		new_file.close();
	}
};
