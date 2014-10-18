#ifndef RAY_TRACER_OBJECTS_H
#define RAY_TRACER_OBJECTS_H


#include "Eigen/Dense"

using namespace Eigen;

const int TRANSLATION = 0;
const int ROTATION = 1;
const int SCALING = 2;

struct camera {
	Vector3d e;
	Vector2d ll, lr, ul, ur;

	camera(int x, int y, int z, int llx, int lly, int lrx, int lry, int ulx, int uly, int urx, int ury) {
		e = Vector3d(x, y, z);
		ll = Vector2d(llx, lly);
		lr = Vector2d(lrx, lry);
		ul = Vector2d(ulx, uly);
		ur = Vector2d(urx, ury);
	}
};

struct sphere
{
	int radius;
	Vector3d center;

	sphere(int cx, int cy, int cz, int r) {
		radius = r;
		center = Vector3d(cx, cy, cz);
	}
};

struct triangle
{
	Vector3d a, b, c;

	triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz) {
		a = Vector3d(ax, ay, az);
		b = Vector3d(bx, by, bz);
		c = Vector3d(cx, cy, cz);
	}
};

struct light
{
	float r, g, b;
};

struct pointLight : light 
{
	Vector3d point;
	int falloff;
};

struct directionalLight : light
{
	Vector3d direction;
};

struct ambientLight : light {}; // this exists purely for naming convenience

struct material
{
	float ambR, ambG, ambB;
	float diffR, diffG, diffB;
	float specR, specG, specB;

};


struct transform
{
	int type; // 0: translation, 1: rotation, 2: scaling
	int x, y, z;
	Vector3d matrix;
	void createMatrix(int x, int y, int z);
};

// Transforms have instatiators that call the create matrix function and assign them to the matrix variable.
// TODO: overload the arithmetic operators on these to enable easy arithmatic with matrices. Eg: allow (matrix * translate) = (matrix * translate.matrix) etc

struct translate : transform
{

	void createMatrix() {
		// TODO: fill in transform matrix
	}

	translate(int tx, int ty, int tz) {
		type = TRANSLATION;
		x = tx;
		y = ty;
		z = tz;
		createMatrix();
	}
};

struct rotation: transform
{
	void createMatrix() {
		// TODO: fill in transform matrix
	}

	rotation(int rx, int ry, int rz) {
		type = ROTATION;
		x = rx;
		y = ry;
		z = rz;
		createMatrix();
	}

	
};

struct scaling: transform
{
	void createMatrix() {
		// TODO: fill in transform matrix
	}

	scaling(int sx, int sy, int sz) {
		type = SCALING;
		x = sx;
		y = sy;
		z = sz;
		createMatrix();
	}
};

#endif  /* RAY_TRACER_OBJECTS_H */



