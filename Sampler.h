#ifndef SAMPLER_H
#define SAMPLER_H

#include "Eigen/Dense"

using namespace Eigen;

class Sampler {
public:
	int numPixX, numPixY;
	Vector3f UL, UR, LR, LL;
	int currX, currY;
	float unitX, unitY;
	 


	// Constructor
	Sampler() {}

	// Constructor
	Sampler(int pixX, int pixY, float ulx, float uly, float urx, float ury, float lrx, float lry, float llx, float lly) {
		numPixX = pixX;
		numPixY = pixY;
		UL << ulx, uly, 0;
		UR << urx, ury, 0;
		LR << lrx, lry, 0;
		LL << llx, lly, 0;
		currX = -1;
		currY = 0;
		unitX = (urx - ulx) / pixX;
		unitY = (lly - uly) / pixY;
	}

	// Returns a point representing a sample for pixel at currX, currY
	Vector3f getSample();

	// Returns the center of the rectangle in WORLD SPACE that corresponds to the pixel at x, y
	Vector3f center(int x, int y);

};

#endif /*SAMPLER_H */

