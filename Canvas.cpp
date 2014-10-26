#include <iostream>
#include <string>

#include "lodepng.h"
#include "Canvas.h"

using namespace std;

void Canvas::addPixel(unsigned char r, unsigned char g, unsigned char b) {
	if(Canvas::currX >= Canvas::width - 1) {
		Canvas::currX = 0;
		Canvas::currY++;
	}
	else {
		Canvas::currX++;
	}
	Canvas::image[4 * Canvas::width * currY + 4 * currX + 0] = r;
	Canvas::image[4 * Canvas::width * currY + 4 * currX + 1] = g;
	Canvas::image[4 * Canvas::width * currY + 4 * currX + 2] = b;
	Canvas::image[4 * Canvas::width * currY + 4 * currX + 3] = 255;
}

void Canvas::addPixel(color c) {
	
}

void Canvas::encode(char* filename) {
	lodepng_encode32_file(filename, Canvas::image, Canvas::width, Canvas::height);
}


