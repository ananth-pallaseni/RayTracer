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
	//Vector3d matrix;
	void createMatrix(int x, int y, int z);
};

struct transform2d : transform
{
	Matrix3d matrix;
	friend Matrix3d operator+(const transform2d &mat1, Matrix3d mat2) {
		return mat1.matrix * mat2;
	}

	friend Matrix3d operator+(Matrix3d mat1, const transform2d &mat2) {
		return mat1 * mat2.matrix;
	}
};

struct transform3d : transform
{
	Matrix4d matrix;
	friend Matrix4d operator+(const transform3d &mat1, Matrix4d mat2) {
		return mat1.matrix * mat2;
	}

	friend Matrix4d operator+(Matrix4d mat1, const transform3d &mat2) {
		return mat1 * mat2.matrix;
	}
};

// Transforms have instatiators that call the create matrix function and assign them to the matrix variable.
// TODO: overload the arithmetic operators on these to enable easy arithmatic with matrices. Eg: allow (matrix * translate) = (matrix * translate.matrix) etc

struct translate2d : transform2d
{

	void createMatrix() {
		matrix << 1, 0, translate2d::x,
				  0, 1, translate2d::y, 
				  0, 0, 1;
	}

	translate2d(int tx, int ty, int tz) {
		translate2d::type = TRANSLATION;
		translate2d::x = tx;
		translate2d::y = ty;
		translate2d::z = tz;
		createMatrix();
	}
};

struct rotation2d: transform2d
{
	void createMatrix() {
		// TODO: fill in transform matrix
	}

	rotation2d(int rx, int ry, int rz) {
		rotation2d::type = ROTATION;
		rotation2d::x = rx;
		rotation2d::y = ry;
		rotation2d::z = rz;
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



