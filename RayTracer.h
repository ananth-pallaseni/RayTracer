
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
	Vector3f e;
	sphere* spheres;
	triangle* triangles;
	int numSpheres;
	int numTriangles;

	RayTracer() {}
	RayTracer(Vector3f eye, sphere* sphList, int numSph, triangle* triList, int numTri) {
		spheres = sphList;
		triangles = triList;
		e = eye;
		numSpheres = numSph;
		numTriangles = numTri;
	}

	ray createRay(Vector3f s);

	Vector3f closest(Vector3f v1, Vector3f v2);

	color shade(Vector3f p);


	// Simple ray trace function, no shadows or anything fancy
	color trace(ray r);
};



#endif /* RAYTRACER_H */

