
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

const int DEPTH_MAX = 1;

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

	Vector3f closest(Vector3f v1, Vector3f v2, Vector3f p);

	bool shadowRay(ray sRay);

	bool pointShadowRay(Vector3f point, Vector3f lightOrigin);

	bool directionalShadowRay(Vector3f point, Vector3f directionToLight);

	Vector3f reflectionRay(Vector3f point, Vector3f normalAtPoint, ray incoming, Vector3f k_refl, int depth);

	color shade(Vector3f pointOnShape, Vector3f normalAtPoint, object shape, int depth, ray incoming) ;


	// Simple ray trace function, no shadows or anything fancy
	color traceRay(ray r, int depth, Vector3f source);

	// Combined trace function
	color trace(Vector3f s);
};



#endif /* RAYTRACER_H */

