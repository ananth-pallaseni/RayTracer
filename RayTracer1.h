#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <fstream>
#include <string>

#include "Sampler.h"
#include "Canvas.h"
#include "RayTracerObjects.h"
#include "AABBTree.h"

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
	vector<boundingBox> *boxes;

	RayTracer() {}
	RayTracer(Vector3f eye, vector<boundingBox>* b) {
		e = eye;
		boxes = b;
	}
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
};

#endif /* RAYTRACER_H */
