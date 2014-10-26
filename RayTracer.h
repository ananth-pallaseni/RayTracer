
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string>

#include "Sampler.h"
#include "Canvas.h"
#include "RayTracerObjects.h"

class RayTracer {
public:
	RayTracer() {}
	RayTracer(Vector3f e)

	ray createRay(Vector3f s) {
		return ray(e, s);
	}

	void trace(ray r) {
		
	}
};



#endif /* RAYTRACER_H */

