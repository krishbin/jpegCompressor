#pragma on
#include<string>

using namespace std;

struct BitString
{
	BitString() = default;
	BitString(uint16_t code_, uint8_t numBits_)
		: code(code_), numBits(numBits_) {}
	uint16_t code;
	uint8_t  numBits;
};



class HuffmanTable {
public:
	bool AC = true;
	int length;
	int CodeCounts[16];
	string CodeValues;

	BitString HCodes[256];

	string intToBin(int n) {
		std::string r;
		while (n != 0) { r = (n % 2 == 0 ? "0" : "1") + r; n /= 2; }
		return r;
	}

	HuffmanTable(bool _AC, int _length, int ccounts[16], string cvalues, int* cvaluesI, int numCodes = -1) {
		length = _length;
		AC = _AC;
		for (int i = 0; i < 16; i++) {
			CodeCounts[i] = ccounts[i];
		}

		CodeValues = cvalues;

		auto huffmanCode = 0;
		for (auto numBits = 1; numBits <= 16; numBits++)
		{
			for (auto i = 0; i < CodeCounts[numBits - 1]; i++) {
				HCodes[*cvaluesI++] = BitString(huffmanCode++, numBits);

			}

			huffmanCode <<= 1;
		}



	}

	HuffmanTable() {
		length = 0x1f;
		AC = true;

		for (int i = 0; i < 16; i++)
			CodeCounts[i] = 0;

		CodeValues = "";

	}
};

class HuffmanTransformer {
public:
	static BitString encode(HuffmanTable table, int value) {
		return table.HCodes[value];
	}
};


