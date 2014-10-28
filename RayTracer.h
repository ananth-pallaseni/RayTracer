
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string>

#include "Sampler.h"
#include "Canvas.h"
#include "RayTracerObjects.h"

using namespace std;

class RayTracer {
public:
	Vector3f e;
	vector<sphere> spheres;
	vector<triangle> triangles;
	vector<pointLight> pointLights;
	vector<directionalLight> directionalLights;
	vector<ambientLight> ambientLights;
	int numSpheres;
	int numTriangles;
	int numPointLights;
	int numDirectionalLights;
	int numAmbientLights;

	RayTracer() {}
	RayTracer(Vector3f eye, vector<sphere> sphList, vector<triangle> triList, vector<pointLight> plList, vector<directionalLight> dlList,
			  vector<ambientLight> alList ) {
		e = eye;
		spheres = sphList;
		triangles = triList;
		numSpheres = spheres.size();
		numTriangles = triangles.size();
		pointLights = plList;
		directionalLights = dlList;
		ambientLights = alList;
		numPointLights = pointLights.size();
		numDirectionalLights = directionalLights.size(); 
		numAmbientLights = ambientLights.size();
	}

	ray createRay(Vector3f s);

	Vector3f closest(Vector3f v1, Vector3f v2);

	color shade(Vector3f pointOnShape, Vector3f normalAtPoint, object shape) ;


	// Simple ray trace function, no shadows or anything fancy
	color traceRay(ray r);

	// Combined trace function
	color trace(Vector3f s);
};



#endif /* RAYTRACER_H */

