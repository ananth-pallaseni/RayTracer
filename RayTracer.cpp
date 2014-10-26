
#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string>

#include "Sampler.h"
#include "Canvas.h"
#include "RayTracerObjects.h"
#include "RayTracer.h"

using namespace std;

ray RayTracer::createRay(Vector3f s) {
	return ray(e, s);
}

Vector3f RayTracer::closest(Vector3f v1, Vector3f v2) {
	float d1 = (v1 - e).norm();
	float d2 = (v2 - e).norm();
	if(d1 < d2) {
		return v1;
	}
	else {
		return v2;
	}

}


color RayTracer::shade(Vector3f p) {
	return color(255, 0, 0);
}


// Simple ray trace function, no shadows or anything fancy
color RayTracer::trace(ray r) {
	Vector3f point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); // init to max value
	Vector3f temp;
	bool hit = false;
	// Check all Spheres
	for(int i = 0; i < numSpheres; i++) {
		Vector3f 
		if(r.intersect(spheres[i], &temp)) {
			hit = true;
			point = closest(point, temp);
		}
	}

	// Check all Triangles
	for(int i = 0; i < numTriangles; i++) {
		Vector3f 
		if(r.intersect(triangles[i], &temp)) {
			hit = true;
			point = closest(point, temp);
		}
	}

	if(hit) {
		return shade(point);
	}
	else {
		return color();
	}
}

