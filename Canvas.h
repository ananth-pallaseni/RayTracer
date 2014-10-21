#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <string>
#include "lodepng.h"

using namespace std;

class Canvas {
public:
	int width, height, currX, currY;
	unsigned char* image;

	Canvas() {};

	Canvas(int w, int h) {
		width = w;
		height = h;
		currX = -1;
		currY = 0;
		image = new unsigned char[width * height * 4];
	}

	void addPixel(unsigned char r, unsigned char g, unsigned char b);
	void encode(char* filename);

};

#endif /* CANVAS_H */
