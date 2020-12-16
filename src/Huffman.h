#pragma on
#include<string>

using namespace std;

class HuffmanTable {
public:
	bool AC = true;
	int length;
	int CodeCounts[16];
	string CodeValues;
	int* CodeLengths;
	int* Codes;

	HuffmanTable(bool _AC, int _length, int ccounts[16], string cvalues, int numCodes = -1) {
		length = _length;
		AC = _AC;
		for (int i = 0; i < 16; i++) {
			CodeCounts[i] = ccounts[i];
		}


		CodeValues = cvalues;

		if (numCodes == -1) {
			numCodes = CodeValues.length();
		}

    CodeLengths = new int[numCodes];
    Codes = new int[numCodes];
		int index = 0;
		for (int ii = 0; ii < 16; ii++) {
			for (int jj = 0; jj < CodeCounts[ii]; jj++) {
				CodeLengths[index] = ii;
				index++;

			}
		}

		int huffmanCodeCounter = 0;
		int codeLengthCounter = 1;

		for (int i = 0; i < numCodes; i++) {
			if (CodeLengths[i] == codeLengthCounter) {
				Codes[i] = huffmanCodeCounter;
				huffmanCodeCounter++;
			}
			else {
				huffmanCodeCounter <<= 1;
				codeLengthCounter++;
			}
		}


	}

	HuffmanTable() {
		length = 0x1f;
		AC = true;

		for (int i = 0; i < 16; i++)
			CodeCounts[i] = 0;

		CodeValues = "";
		CodeLengths[0] = 0;
		Codes[0] = 0;
	}
  ~HuffmanTable(){
    if(CodeLengths){
    delete[] CodeLengths;
    CodeLengths = NULL;
    }
    if(Codes){
    delete[] Codes;
    Codes = NULL;
    }
  }
};

class HuffmanTransformer {
public:
	static string intToBin(int n) {
		std::string r;
		while (n != 0) { r = (n % 2 == 0 ? "0" : "1") + r; n /= 2; }
		return r;
	}

	static string encode(HuffmanTable table, int value) {
		for (int i = 0; i < table.CodeValues.length(); i++) {
			if (value == (int)table.CodeValues.at(i)) {
				return intToBin(table.Codes[i]);
			}
		}
    return "";
	};
};


