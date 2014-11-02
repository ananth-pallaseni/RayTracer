
#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string>

#include <math.h>
#include "Sampler.h"
#include "Canvas.h"
#include "RayTracerObjects.h"
#include "RayTracer.h"

using namespace std;

ray RayTracer::createRay(Vector3f s) {
	return ray(e, s);
}

Vector3f RayTracer::closest(Vector3f v1, Vector3f v2, Vector3f p) {
	float d1 = (v1 - p).norm();
	float d2 = (v2 - p).norm();
	if(d1 < d2) {
		return v1;
	}
	else {
		return v2;
	}

}

Vector3f unit(Vector3f v) {
	return v / v.norm();
}

Vector3f vMul(Vector3f v1, Vector3f v2) {
	Vector3f vv(v1(0) * v2(0), v1(1) * v2(1), v1(2) * v2(2));
	return vv;
}

Vector3f clamp(Vector3f v) {
	Vector3f vv(v(0) > 1 ? 1 : v(0), v(1) > 1 ? 1 : v(1), v(2) > 1 ? 1 : v(2));
	return vv;
}

Vector3f sphereNormal(Vector3f pointOnShape, sphere shape) {
	return unit(pointOnShape - shape.center);
}

// Flat Shading:
Vector3f triangleNormal(Vector3f pointOnShape, triangle shape, ray r) {
	Vector3f side1 = shape.a - shape.b;
	Vector3f side2 = shape.a - shape.c;
	Vector3f planeNormal = side1.cross(side2);
	if((-r.sMinusE).dot(planeNormal) < 0) {
		planeNormal = -planeNormal;
	}
	planeNormal = planeNormal / planeNormal.norm();
	return planeNormal;

}

Vector3f diffuse(Vector3f n, Vector3f l, Vector3f k_diffuse, Vector3f k_light) {
	float cosine = n.dot(l);
	if(cosine < 0) {
		cosine = 0;
	}
	Vector3f v = cosine * k_light;
	Vector3f v1 = vMul(k_diffuse, v);
	v1 = clamp(v1);
	return v1;
}

Vector3f ambient(Vector3f k_light, Vector3f k_ambient) {
	return vMul(k_light, k_ambient);
}

Vector3f specular(Vector3f n, Vector3f l, Vector3f e, Vector3f k_specular, Vector3f k_light, float p) {
	Vector3f notL = -l;
	Vector3f r = notL + (2 * n.dot(l) * n);
	float dotRE = r.dot(unit(-e));
	if(dotRE < 0) {
		dotRE = 0;
	}
	Vector3f v = pow(dotRE, p) * k_light;
	return vMul(k_specular, v);
}

// Creates a shadow ray, and compares it with all objects 
bool RayTracer::shadowRay(ray sRay) {
	// ray from point -> lightOrigin
	// ray r(point, lightOrigin);
	float t;
	for(int i = 0; i < numSpheres; i++) {
		if(sRay.intersect(spheres[i])) {
			return true;
		}
	}
	for(int i = 0; i < numTriangles; i++) {
		if(sRay.intersect(triangles[i], &t)) {
			if(t > 0.1f) { // only return if the object intersects the ray in the positive direction.
				return true;
			}
		}
	}
	return false;
}

bool RayTracer::pointShadowRay(Vector3f point, Vector3f lightOrigin) {
	ray r(point, lightOrigin);
	return shadowRay(r);
}

bool RayTracer::directionalShadowRay(Vector3f point, Vector3f directionToLight) {
	ray r(point, directionToLight, true);
	return shadowRay(r);
}	


Vector3f RayTracer::shade(Vector3f pointOnShape, Vector3f normalAtPoint, material mat) {
	Vector3f rgb(0, 0, 0);
	for(int i = 0; i < numPointLights; i++) {
		pointLight pl = pointLights[i];
		if(!pointShadowRay(pointOnShape, pl.point)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = unit(pl.point - pointOnShape);
	
			rgb = rgb + diffuse(normalAtPoint, lightDirection, mat.diff, pl.l());
			rgb = rgb + specular(normalAtPoint, lightDirection, incoming.sMinusE, mat.spec, pl.l(), shape.mat.phongExp);
		}
		rgb = rgb + ambient(pl.l(), mat.amb);
	}

	for(int i = 0; i < numDirectionalLights; i++) {
		directionalLight dl = directionalLights[i];
		if(!directionalShadowRay(pointOnShape, -dl.direction)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = -unit(dl.direction);

			rgb = rgb + diffuse(normalAtPoint, lightDirection, mat.diff, dl.l());
			rgb = rgb + specular(normalAtPoint, lightDirection, incoming.sMinusE, mat.spec, dl.l(), mat.phongExp);
		}
		rgb = rgb + ambient(dl.l(), mat.amb);
	}

	for(int i = 0; i < numAmbientLights; i++) {
		ambientLight al = ambientLights[i];
		rgb = rgb + ambient(al.l(), mat.amb);
	}

	rgb = clamp(rgb);
	return rgb;
}


Vector3f RayTracer::reflectionRay(Vector3f point, Vector3f normalAtPoint, ray incoming, Vector3f k_refl, int depth) {
	// Reflect ray about normal:
	ray refl(point, incoming.sMinusE - 2 * normalAtPoint * (incoming.sMinusE.dot(normalAtPoint)), true);
	Vector3f rgb(0, 0, 0);
	while(depth > 0) {
		depth--;
		hitResult result;
		if(traceRay(refl, point, &result)) {
			rgb = rgb + shade(result.point, result.point, result.mat);
			if(!(result.mat.refl(0) == 0 && result.mat.refl(1) == 0 && result.mat.refl(2) == 0)) {
				rgb = rgb + reflectionRay(result.point, result.normal, refl, result.mat.refl, depth);
			}
		}
	}
	

	return vMul(k_refl, rgb);
}

// Simple ray trace function, no shadows or anything fancy
bool RayTracer::traceRay(ray r, Vector3f source, hitResult* result) {
	Vector3f point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); // init to max value
	Vector3f temp;
	bool hit = false;
	Vector3f normal;
	object shape;

	// Check all Spheres
	for(int i = 0; i < numSpheres; i++) {
		if(r.intersect(spheres[i], &temp, &tempT)) {
			hit = true;
			temp = closest(point, temp, source);
			if(point != temp) {
				normal = sphereNormal(temp, spheres[i]);
				shape = spheres[i];
				point = temp;
			}
		}
	}

	// Check all Triangles
	for(int i = 0; i < numTriangles; i++) {
		if(r.intersect(triangles[i], &temp)) {
			hit = true;
			temp = closest(point, temp, source);
			if(point != temp) {
				normal = triangleNormal(temp, triangles[i], r);
				shape = triangles[i];
				point = temp;
			}
		}
	}

	if(hit) {
		result->point = point;
		result->normal = normal;
		result->mat = shape.mat;
		return true;
	}
	else {
		return false; 
	}
}

color RayTracer::trace(Vector3f s) {
	color rgb(0, 0, 0);
	ray r = createRay(s);
	hitResult result;
	if( traceRay(r, 0, e, &result)) {
		rgb = rgb + shade(result.point, result.point, result.mat);
		if(!(result.mat.refl(0) == 0 && result.mat.refl(1) == 0 && result.mat.refl(2) == 0)) {
			rgb = rgb + reflectionRay(result.point, result.normal, refl, result.mat.refl, DEPTH_MAX);
		}
	}
	return traceRay(r, 0, e);
}

