#pragma once
template<class T>
class Chunk {


public:
	T pixelValues[8][8];
	Chunk(T values[8][8]) {
		for (int i = 0; i < 8; i++) {
			for (int ii = 0; ii < 8; ii++) {
				pixelValues[i][ii] = values[i][ii];
			}
		}

	}
	Chunk(const T values[8][8]) {
		for (int i = 0; i < 8; i++) {
			for (int ii = 0; ii < 8; ii++) {
				pixelValues[i][ii] = values[i][ii];
			}
		}

	}

	T getNthValue(int n) {
		if (n >= 64) {
			n = 64;
		}
		if (n < 0) {
			n = 0;
		}
		return pixelValues[(int)(n / 8)][8 % n];
	}

	/*T getZigZagNthValue(int n) {
		if (n >= 64) {
			n = 64;
		}
		if (n < 0) {
			n = 0;
		}
		return pixelValues[(int)(n / 8)][8 % n];
	}*/

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

