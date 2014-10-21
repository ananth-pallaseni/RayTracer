#ifndef SAMPLER_H
#define SAMPLER_H

#include "Eigen/Dense"

using namespace Eigen;

class Sampler {
public:
	int numPixX, numPixY;
	Vector2f UL, UR, LR, LL;
	int currX, currY;
	float unitX, unitY;
	 


	// Constructor
	Sampler() {}

	// Constructor
	Sampler(int pixX, int pixY, float ulx, float uly, float urx, float ury, float lrx, float lry, float llx, float lly) {
		numPixX = pixX;
		numPixY = pixY;
		UL << ulx, uly;
		UR << urx, ury;
		LR << lrx, lry;
		LL << llx, lly;
		currX = -1;
		currY = 0;
		unitX = (urx - ulx) / pixX;
		unitY = (lly - uly) / pixY;
	}

	// Returns a point representing a sample for pixel at currX, currY
	Vector2f getSample();

	// Returns the center of the rectangle in WORLD SPACE that corresponds to the pixel at x, y
	Vector2f center(int x, int y);

};

#endif /*SAMPLER_H */

