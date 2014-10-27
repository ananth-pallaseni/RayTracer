
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

/*Vector3f diffuse(Vector3f n, Vector3f l, Vector3f k_diffuse, Vector3f k_light) {
	float cosine = dot(n, l);
	Vector3f v = sMul(k_light, max(0, cosine));
	Vector3f v1 = vMul(k_diffuse, v);
	clamp(v1);
	return v1;
}

Vector3f ambient(Vector3f k_light, Vector3f k_ambient) {
	return vMul(k_light, k_ambient);
}

Vector3f specular(Vector3f n, Vector3f l, Vector3f e, Vector3f k_specular, Vector3f k_light, float p) {
	Vector3f notL = negate(l);
	Vector3f r = vAdd(notL, sMul(n, 2 * dot(n, l)));
	Vector3f v = sMul(k_light, pow(max(dot(r, e), 0), p));
	return vMul(k_specular, v);
}*/

color RayTracer::shade(Vector3f p) {
	int r = 0;
	int g = 0;
	int b = 0;
	for(int i = 0; i < numPointLights; i++) {

	}

	for(int i = 0; i < numDirectionalLights; i++) {
		
	}
}


// Simple ray trace function, no shadows or anything fancy
color RayTracer::traceRay(ray r) {
	Vector3f point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); // init to max value
	Vector3f temp;
	bool hit = false;
	// Check all Spheres
	for(int i = 0; i < numSpheres; i++) {
		if(r.intersect(spheres[i], &temp)) {
			hit = true;
			point = closest(point, temp);
		}
	}

	// Check all Triangles
	for(int i = 0; i < numTriangles; i++) {
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

color RayTracer::trace(Vector3f s) {
	ray r = createRay(s);
	return traceRay(r);
}

