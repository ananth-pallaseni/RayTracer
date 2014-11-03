
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
	Vector3f normal = pointOnShape - shape.center;
	Matrix4f m = shape.objToWorld;
	Matrix3f normTransform;
	normTransform << m(1, 1) * m(2, 2) - m(1, 2) * m(2 , 1), m(1, 2) * m(3,1) - m(1, 0) * m(2 ,2), m(1,0)*m(2,1) - m(1,1)*m(2,0),
					 m(0,2)*m(2,1) - m(0,1)*m(2,2), m(0,0)*m(2,2)-m(0,2)*m(2,0), m(0,1)*m(2,0)-m(0,0)*m(2,1),
					 m(0,1)*m(1,2)-m(0,1)*m(1,1), m(0,2)*m(1,0)-m(0,0)*m(1,2), m(0,0)*m(1,1)-m(0,1)*m(1,0);
	normal = normTransform * normal;
	return unit(normal);
}

// Flat Shading:
Vector3f triangleNormal(Vector3f pointOnShape, triangle shape, ray r) {
	/*Vector3f side1 = shape.a - shape.b;
	Vector3f side2 = shape.a - shape.c;
	Vector3f normal(side1(1) * side2(2) - side1(2) * side2(1), 
					side1(2) * side2(0) - side1(0) * side2(2),
					side1(0) * side2(1) - side1(1) * side2(0));
	Vector3f unitNormal = unit(normal);
	if(unit(r.sMinusE).dot(unitNormal) < 0) {
		unitNormal = -unitNormal;
	}
	return unitNormal;*/
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
//bool RayTracer::shadowRay(Vector3f point, Vector3f lightOrigin) {
bool RayTracer::shadowRay(ray sRay) {
	// ray from point -> lightOrigin
	//ray r(point, lightOrigin);
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

bool RayTracer::pointShadowRay(Vector3f point, Vector3f lightOrigin, float* dist) {
	ray r(point, lightOrigin);
	*dist = (point - lightOrigin).norm();
	return shadowRay(r);
}

bool RayTracer::directionalShadowRay(Vector3f point, Vector3f directionToLight) {
	ray r(point, directionToLight, true);
	return shadowRay(r);
}	

Vector3f RayTracer::reflectionRay(Vector3f point, Vector3f normalAtPoint, ray incoming, Vector3f k_refl, int depth) {
	// Reflect ray about normal:
	ray refl(point, incoming.sMinusE - 2 * normalAtPoint * (incoming.sMinusE.dot(normalAtPoint)), true);
	color cTemp = traceRay(refl, depth + 1, point);
	Vector3f c(cTemp.r * k_refl(0), cTemp.g * k_refl(1), cTemp.b * k_refl(2));
	/*if(isTriangle) {
		for(int i = 0; i < 3; i++) {
			if(k_refl(i) == 0) {
				k_refl(i) = 1;
			}
			else if(k_refl(i) == 1) {
				k_refl(i) = 0;
			}
		}
		c = Vector3f(cTemp.r * (1 - k_refl(0)), cTemp.g * (1 - k_refl(1)), cTemp.b * (1 - k_refl(2)) );
	}
	else{
		c = Vector3f(cTemp.r * k_refl(0), cTemp.g * k_refl(1), cTemp.b * k_refl(2) );	
	}*/
	c = c / 255; // as color rgb values are (0->255)

	return c;
}



color RayTracer::shade(Vector3f pointOnShape, Vector3f normalAtPoint, material mat, int depth, ray incoming) {
	Vector3f rgb(0, 0, 0);
	for(int i = 0; i < numPointLights; i++) {
		pointLight pl = pointLights[i];
		float dist;
		if(!pointShadowRay(pointOnShape, pl.point, &dist)) {
			// lightDirection is unit vector pointing TO light
			Vector3f lightDirection = unit(pl.point - pointOnShape);
	
			rgb = rgb + diffuse(normalAtPoint, lightDirection, mat.diff, pl.l(dist));
			rgb = rgb + specular(normalAtPoint, lightDirection, incoming.sMinusE, mat.spec, pl.l(), mat.phongExp);
		}
		if(depth < DEPTH_MAX && !(mat.refl(0) ==  0 && mat.refl(1) == 0 && mat.refl(2) == 0)) {
			rgb = rgb + reflectionRay(pointOnShape, normalAtPoint, incoming, mat.refl, depth);
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
		if(depth < DEPTH_MAX && !(mat.refl(0) ==  0 && mat.refl(1) == 0 && mat.refl(2) == 0)) {
			rgb = rgb + reflectionRay(pointOnShape, normalAtPoint, incoming, mat.refl, depth);
		}
		rgb = rgb + ambient(dl.l(), mat.amb);
	}

	for(int i = 0; i < numAmbientLights; i++) {
		ambientLight al = ambientLights[i];
		rgb = rgb + ambient(al.l(), mat.amb);
	}

	rgb = clamp(rgb);
	return color(rgb);
}


// Simple ray trace function, no shadows or anything fancy
color RayTracer::traceRay(ray r, int depth, Vector3f source) {
	cout << "START TRAVERSE" << endl;
	hitResult result;
	if(AABBRoot->rayTraverse(&r, &result)) {
		cout << "SHADE" << endl;
		return shade(result.point, result.normal, result.mat, depth, r);
	}
	else {
		return color();
	}
}

color RayTracer::trace(Vector3f s) {
	ray r = createRay(s);
	return traceRay(r, 0, e);
}

