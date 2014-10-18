//#include "stdafx.h"
#include "lodepng.h"
#include "Eigen/Dense"
#include "Sampler.h"

using namespace Eigen;

// Returns a point representing a sample for pixel at currX, currY
Vector2d Sampler::getSample() {
	if (currX > numPixX) {
		Sampler::currX = 0;
		Sampler::currY++;
	}
	else {
		Sampler::currX++;
	}
	return Sampler::center(currX, currY);
}

// Returns the center of the rectangle in WORLD SPACE that corresponds to the pixel at x, y
Vector2d Sampler::center(int x, int y) {
	//Assuming u and v  go from 0 to 1:
	float u = x / numPixX;
	float v = y / numPixY;
	return u*(v*LL + (1 - v)*UL) + (1 - u)*(v*LR + (1 - v)*UR);
}


