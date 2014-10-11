#include "stdafx.h"
#include "lodepng.h"
#include "Eigen/Dense"

using namespace Eigen;

class Sampler {
public:
	int numPixX, numPixY;
	Vector2d UL, UR, LR, LL;
	int currX = -1;
	int currY = 0;

	Sampler() { };

	Sampler(int pixX, int pixY, int ulx, int uly, int urx, int ury, int lrx, int lry, int llx, int lly) {
		numPixX = pixX;
		numPixY = pixY;
		UL << ulx, uly;
		UR << urx, ury;
		LR << lrx, lry;
		LL << llx, lly;
	}

	// Returns a point representing a sample for pixel at currX, currY
	Vector2d getSample() {
		if (currX > numPixX) {
			currX = 0;
			currY++;
		}
		else {
			currX++;
		}
		return center(currX, currY);
	}

	// Returns the center of the rectangle in WORLD SPACE that corresponds to the pixel at x, y
	Vector2d center(int x, int y) {
		//Assuming u and v  go from 0 to 1:
		float u = x / numPixX;
		float v = y / numPixY;
		return u*(v*LL + (1 - v)*UL) + (1 - u)*(v*LR + (1 - v)*UR);
	}
};