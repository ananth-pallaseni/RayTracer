//#include "stdafx.h"
#include "lodepng.h"
#include "Eigen/Dense"
#include "Sampler.h"

using namespace Eigen;

// Returns a point representing a sample for pixel at currX, currY
Vector3f Sampler::getSample() {
	if (Sampler::currX >= Sampler::numPixX - 1) {
		Sampler::currX = 0;
		Sampler::currY++;
	}
	else {
		Sampler::currX++;
	}
	return Sampler::center(currX, currY);
}

// Returns the center of the rectangle in WORLD SPACE that corresponds to the pixel at x, y
Vector3f Sampler::center(int x, int y) {
	//Assuming u and v  go from 0 to 1:
	Vector2f v(float(x*Sampler::unitX), float(y*Sampler::unitY));
	return Sampler::UL + v + Vector2f(0.5f*Sampler::unitX, 0.5f*Sampler::unitY, 0);
	//return u*(v*LL + (1 - v)*UL) + (1 - u)*(v*LR + (1 - v)*UR);
}


