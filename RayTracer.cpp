
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
		if(r.intersect(spheres[i])) {
			return true;
		}
	}
	for(int i = 0; i < numTriangles; i++) {
		if(r.intersect(triangles[i], &t)) {
			if(t > 0.01f) { // only return if the object intersects the ray in the positive direction.
				return true;
			}
		}
	}
	return false;

}

Vector3f RayTracer::reflectionRay(Vector3f point, Vector3f normalAtPoint, ray incoming, Vector3f k_refl, int depth) {
	// Reflect ray about normal:
	ray refl(point, incoming.sMinusE - 2 * normalAtPoint * (incoming.sMinusE.dot(normalAtPoint)));
	color cTemp = traceRay(refl, depth + 1, point);
	Vector3f c(cTemp.r * k_refl(0), cTemp.g * k_refl(1), cTemp.b * k_refl(2) );
	if((cTemp.b > 0 || cTemp.r > 0 || cTemp.g > 0)) {
		
	}
	cout << "///////////////// REFL FROM POINT: " << point(0) << ", " << point(1) << ", " << point(2) << endl;
		cout << "///////////////// REFL USES VALUES: " << k_refl(0) << ", " << k_refl(1) << ", " << k_refl(2) << endl << endl;
	return c;
}



color RayTracer::shade(Vector3f pointOnShape, Vector3f normalAtPoint, object shape, int depth, ray incoming) {
	Vector3f rgb(0, 0, 0);
	for(int i = 0; i < numPointLights; i++) {
		pointLight pl = pointLights[i];
		if(!shadowRay(pointOnShape, pl.point)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = unit(pl.point - pointOnShape);
	
			Vector3f ttt(50, 50, 50);
			rgb = rgb + diffuse(normalAtPoint, lightDirection, shape.mat.diff, pl.l());
			rgb = rgb + specular(normalAtPoint, lightDirection, e, shape.mat.spec, pl.l(), shape.mat.phongExp);
		}
		if(depth < DEPTH_MAX ) {
			rgb = rgb + reflectionRay(pointOnShape, normalAtPoint, incoming, shape.mat.refl, depth);
		}
		rgb = rgb + ambient(pl.l(), shape.mat.amb);
	}

	for(int i = 0; i < numDirectionalLights; i++) {
		directionalLight dl = directionalLights[i];
		if(!shadowRay(pointOnShape, dl.direction)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = -unit(dl.direction);

			rgb = rgb + diffuse(normalAtPoint, lightDirection, shape.mat.diff, dl.l());
			rgb = rgb + specular(normalAtPoint, lightDirection, e, shape.mat.spec, dl.l(), shape.mat.phongExp);
		}
		if(depth < DEPTH_MAX) {
			rgb = rgb + reflectionRay(pointOnShape, normalAtPoint, incoming, shape.mat.refl, depth);
		}
		rgb = rgb + ambient(dl.l(), shape.mat.amb);
	}

	for(int i = 0; i < numAmbientLights; i++) {
		ambientLight al = ambientLights[i];
		rgb = rgb + ambient(al.l(), shape.mat.amb);
	}

	rgb = clamp(rgb);
	//cout << rgb << endl << endl;
	return color(rgb);
}


// Simple ray trace function, no shadows or anything fancy
color RayTracer::traceRay(ray r, int depth, Vector3f source) {
	Vector3f point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); // init to max value
	Vector3f temp;
	bool hit = false;
	////// new
	Vector3f normal;
	object shape;
	////// endnew
	
	// REMOVE THIS:
	float tempT;

	// Check all Spheres
	for(int i = 0; i < numSpheres; i++) {
		if(r.intersect(spheres[i], &temp, &tempT)) {
			hit = true;
			//point = closest(point, temp); OLD
			///////////////////////new
			temp = closest(point, temp, source);
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
			temp = closest(point, temp, source);
			if(point != temp) {
				normal = triangleNormal(temp, triangles[i], r);
				shape = triangles[i];
				point = temp;
			}
			
			//////////////////////endnew
		}
	}

	if(hit) {
		// NEW:
		color cq = shade(point, normal, shape, depth, r);
		if ( depth == 1) {
			string hitcol = "RED";
			float refl1, refl2, refl3;
			refl1 = 0;
			refl2 = 0;
			refl3 = 0;
			if(point(0) > 0) {
				hitcol = "BLUE";
			}
			if(r.e(0) < 0) {
				refl2 = 0.5f;
				refl3 = 0.5f;
			}
			color adjC(cq.r * refl1, cq.g * refl2, cq.b * refl3);
			cout << "HIT "<< hitcol << " AT, POINT: " << endl << point << endl << "t : " << tempT << endl;
			cout << "MATERIAL REFL PROPS: " << refl1 << ", " << refl2 << ", " << refl3 << endl;
			cout << "RAW COLOR: " << cq << endl << "FINAL COLOR: " << adjC << endl;
			cout << "RAY START: " << endl << r.e << endl << "RAY DIR: " << endl << r.sMinusE << endl << endl; 
		}
		// ENDNEW

		return cq;
	}
	else {
		return color(); // BLACK
	}
}

color RayTracer::trace(Vector3f s) {
	ray r = createRay(s);
	return traceRay(r, 0, e);
}

