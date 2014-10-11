#ifndef SAMPLER_H
#define SAMPLER_H

#include "Eigen\Dense"

using namespace Eigen;

class Sampler {
public:
	int numPixX, numPixY;
	Vector2d UL, UR, LR, LL;
	int currX, currY;

	Sampler();

	Sampler(int pixX, int pixY, int ulx, int uly, int urx, int ury, int lrx, int lry, int llx, int lly);

	// Returns a point representing a sample for pixel at currX, currY
	Vector2d getSample();

	// Returns the center of the rectangle in WORLD SPACE that corresponds to the pixel at x, y
	Vector2d center(int x, int y);
};

#endif //SAMPLER_H 

