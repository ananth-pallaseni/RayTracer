#ifndef RAY_TRACER_OBJECTS_H
#define RAY_TRACER_OBJECTS_H


#include "Eigen/Dense"

using namespace Eigen;

const int TRANSLATION = 0;
const int ROTATION = 1;
const int SCALING = 2;

const int SPHERE = 0;
const int TRIANGLE = 1;

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

// Objects : //////////////////////////////////////////////////////////////////////////////

struct object 
{
	int type;
};

struct sphere : object
{
	int radius;
	Vector3d center;

	sphere(int cx, int cy, int cz, int r) {
		radius = r;
		center = Vector3d(cx, cy, cz);
		type = SPHERE;
	}

};

struct triangle : object
{
	Vector3d a, b, c;

	triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz) {
		a = Vector3d(ax, ay, az);
		b = Vector3d(bx, by, bz);
		c = Vector3d(cx, cy, cz);
		type = TRIANGLE;
	}
};

// End Objects /////////////////////////////////////////////////////////////////////////////

struct ray
{
	Vector3f e;
	//Vector3f sample;
	Vector3f sMinusE;

	ray(Vector3f eye, Vector3f s) {
		e = eye;
		//sample = s;
		sMinusE = s - e;
	}

	Vector3f p(float t) {
		return e + t * (sMinusE);
	}

	bool intersect(object o, Vector3f* point) {
		switch(o.type) {
			case SPHERE: 
			{
				// Check Discriminant:
				//Vector3f c = ((sphere) o).center;
				//Vector3f c = (dynamic_cast<sphere>(o)).center;
				sphere sph = o;
				// A = sMinusE . sMinusE
				float A = sMinusE.dot(sMinusE);
				// B = 2 * sMinusE . (e - c)
				float B = 2 * sMinusE .dot(( e - c ));
				// C = (e - c) . (e - c) - r^2
				float C = ( e - c ).dot( ( e - c ) ) - ( ((sphere) o).radius * ((sphere) o).radius );
				float discriminant = B*B - 4*A*C;
				if (discriminant >= 0) {
					// Only use negative value of discriminant, as this will be closer to the plane
					float t = (-B - sqrt(discriminant)) / (2 * A);
					*point = p(t);
					return true;
				}
				return false;
				break;
			}

			case TRIANGLE:
			{
				triangle tri = (triangle) o;

				// Set up matrix A:
				Matrix3f A;
				A(0, 0) = tri.a(0) - tri.b(0);
				A(0, 1) = tri.a(0) - tri.c(0);
				A(0, 2) = sMinusE(0);

				A(1, 0) = tri.a(1) - tri.b(1);
				A(1, 1) = tri.a(1) - tri.c(1);
				A(1, 2) = sMinusE(1);

				A(2, 0) = tri.a(2) - tri.b(2);
				A(2, 1) = tri.a(2) - tri.c(2);
				A(2, 2) = sMinusE(2);

				// Setri up matririx B:
				Vectrior3f B;
				B(0) = tri.a(0) - e(0);
				B(1) = tri.a(1) - e(1);
				B(2) = tri.a(2) - e(2);

				// Store these vals to save computation:
				float eihf = A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1);
				float gfdi = A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2);
				float dheg = A(0, 1) * A(1, 2) - A(1, 1) * A(0, 2);
				float akjb = A(0, 0) * B(1) - B(0) * A(1, 0);
				float jcal = B(0) * A(2, 0) - A(0, 0) * B(2);
				float blkc = A(1, 0) * B(2) - B(1) * A(2, 0);

				// By Cramers Rule:
				float M = A(0, 0) * eihf + A(1, 0) * gfdi + A(2, 0) * dheg;
				float gamma = (A(2, 2) * akjb + A(1, 2) * jcal + A(0, 2) * blkc) / M;
				if (gamma < 0 || gamma > 1) {
					return false;
				}
				float beta = (B(0) * eihf + B(1) * gfdi + B(2) * dheg) / M;
				if (beta < 0 || beta > 1 - gamma) {
					return false;
				}
				float t = (A(2, 1) * akjb + A(1, 1) * jcal + A(0, 1) * blkc) / M;
				*point = p(t);
				return true;
				break;
			}

			default:
				return false;

		}
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



