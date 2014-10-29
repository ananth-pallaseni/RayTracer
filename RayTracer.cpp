
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
	Vector3f normal(side1(1) * side2(2) - side1(2) * side2(1), 
					side1(2) * side2(0) - side1(0) * side2(2),
					side1(0) * side2(1) - side1(1) * side2(0));
	Vector3f unitNormal = unit(normal);
	if(unit(r.sMinusE).dot(unitNormal) < 0) {
		unitNormal = -unitNormal;
	}
	return unitNormal;

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
	cout << vMul(k_light, k_ambient) << endl << endl;
	return vMul(k_light, k_ambient);
}

Vector3f specular(Vector3f n, Vector3f l, Vector3f e, Vector3f k_specular, Vector3f k_light, float p) {
	Vector3f notL = -l;
	Vector3f r = notL + (2 * n.dot(l) * n);
	float dotRE = r.dot(unit(e));
	if(dotRE < 0) {
		dotRE = 0;
	}
	Vector3f v = pow(dotRE, p) * k_light;
	return vMul(k_specular, v);
}

// Creates a shadow ray, and compares it with all objects 
bool RayTracer::shadowRay(Vector3f point, Vector3f lightOrigin) {
	// ray from point -> lightOrigin
	ray r(point, lightOrigin);
	float t;
	for(int i = 0; i < numSpheres; i++) {
		if(r.intersect(spheres[i], &t)) {
			if(t > 0.1f) { // only return if the object intersects the ray in the positive direction.
				return true;
			}
		}
	}
	for(int i = 0; i < numTriangles; i++) {
		if(r.intersect(triangles[i], &t)) {
			if(t > 0.1f) { // only return if the object intersects the ray in the positive direction.
				return true;
			}
		}
	}
	return false;

}



color RayTracer::shade(Vector3f pointOnShape, Vector3f normalAtPoint, object shape) {
	Vector3f rgb(0.3f, 0.15f, 0);
	/*for(int i = 0; i < numPointLights; i++) {
		pointLight pl = pointLights[i];
		if(!shadowRay(pointOnShape, pl.point)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = unit(pl.point - pointOnShape);
	
			Vector3f ttt(50, 50, 50);
			rgb = rgb + diffuse(normalAtPoint, lightDirection, shape.mat.diff, pl.l());
			rgb = rgb + specular(normalAtPoint, lightDirection, e, shape.mat.spec, pl.l(), shape.mat.phongExp);
			rgb = rgb + ambient(pl.l(), shape.mat.amb);
		}
	}

	for(int i = 0; i < numDirectionalLights; i++) {
		directionalLight dl = directionalLights[i];
		if(!shadowRay(pointOnShape, dl.direction)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = -unit(dl.direction);

			rgb = rgb + diffuse(normalAtPoint, lightDirection, shape.mat.diff, dl.l());
			rgb = rgb + specular(normalAtPoint, lightDirection, e, shape.mat.spec, dl.l(), shape.mat.phongExp);
			rgb = rgb + ambient(dl.l(), shape.mat.amb);
		}
	}

	for(int i = 0; i < numAmbientLights; i++) {
		ambientLight al = ambientLights[i];
		rgb = rgb + ambient(al.l(), shape.mat.amb);
	}

	rgb = clamp(rgb);*/
	//cout << rgb << endl << endl;
	return color(rgb);
}


// Simple ray trace function, no shadows or anything fancy
color RayTracer::traceRay(ray r) {
	Vector3f point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); // init to max value
	Vector3f temp;
	bool hit = false;
	////// new
	Vector3f normal;
	object shape;
	////// endnew

	// Check all Spheres
	for(int i = 0; i < numSpheres; i++) {
		if(r.intersect(spheres[i], &temp)) {
			hit = true;
			//point = closest(point, temp); OLD
			///////////////////////new
			temp = closest(point, temp);
			if(point != temp) {
				normal = sphereNormal(temp, spheres[i]);
				shape = spheres[i];
				point = temp;
			}
			
			//////////////////////endnew
		}
	}

	// Check all Triangles
	for(int i = 0; i < numTriangles; i++) {
		if(r.intersect(triangles[i], &temp)) {
			hit = true;
			//point = closest(point, temp); OLD
			///////////////////////new
			temp = closest(point, temp);
			if(point != temp) {
				normal = triangleNormal(temp, triangles[i], r);
				shape = triangles[i];
				point = temp;
			}
			
			//////////////////////endnew
		}
	}

	if(hit) {
		return shade(point, normal, shape);
	}
	else {
		return color();
	}
}

color RayTracer::trace(Vector3f s) {
	ray r = createRay(s);
	return traceRay(r);
}

