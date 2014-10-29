#ifndef RAY_TRACER_OBJECTS_H
#define RAY_TRACER_OBJECTS_H


#include "Eigen/Dense"
#include <cmath>
#include <iostream>

using namespace std;
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


struct transform
{
	int type; // 0: translation, 1: rotation, 2: scaling
	float x, y, z;
	//Vector3d matrix;
	void createMatrix(float x, float y, float z);
};

struct transform2d : transform
{
	Matrix3f matrix;
	friend Matrix3f operator+(const transform2d &mat1, Matrix3f mat2) {
		return mat1.matrix * mat2;
	}

	friend Matrix3f operator+(Matrix3f mat1, const transform2d &mat2) {
		return mat1 * mat2.matrix;
	}
};

struct transform3d : transform
{
	Matrix4f matrix;
	Matrix4f inverse;
	friend Matrix4f operator*(const transform3d &mat1, Matrix4f mat2) {
		return mat1.matrix * mat2;
	}

	friend Matrix4f operator*(Matrix4f mat1, const transform3d &mat2) {
		return mat1 * mat2.matrix;
	}
	friend Matrix4f operator*(const transform3d &mat1, const transform3d &mat2) {
		return mat1.matrix * mat2.matrix;
	}

};

// Transforms have instatiators that call the create matrix function and assign them to the matrix variable.
// TODO: overload the arithmetic operators on these to enable easy arithmatic with matrices. Eg: allow (matrix * translate) = (matrix * translate.matrix) etc

struct translate2d : transform2d
{

	void createMatrix() {
		matrix << 1, 0, x,
				  0, 1, y, 
				  0, 0, 1;
	}

	translate2d(float tx, float ty, float tz) {
		translate2d::type = TRANSLATION;
		translate2d::x = tx;
		translate2d::y = ty;
		translate2d::z = tz;
		createMatrix();
	}
};

struct translate : transform3d
{
	void createMatrix() {
		matrix << 1, 0, 0, x,
				  0, 1, 0, y, 
				  0, 0, 1, z,
				  0, 0, 0, 1;

		inverse << 1, 0, 0, -x,
				   0, 1, 0, -y, 
				   0, 0, 1, -z,
				   0, 0, 0, 1;
	}

	translate(float tx, float ty, float tz) {
		type = TRANSLATION;
		x = tx;
		y = ty;
		z = tz;
		createMatrix();
	}
};


struct scale : transform3d
{
	void createMatrix() {
		matrix << x, 0, 0, 0,
				  0, y, 0, 0,
				  0, 0, z, 0, 
				  0, 0, 0, 1;

		inverse << 1.0/x, 0, 0, 0,
				   0, 1.0/y, 0, 0,
				   0, 0, 1.0/z, 0, 
				   0, 0, 0, 1;
	}

	scale(float sx, float sy, float sz) {
		type = SCALING;
		x = sx;
		y = sy;
		z = sz;
		createMatrix();
	}
};


struct rotation: transform3d
{

	Matrix3f rCross(Vector3f rHat) {
		Matrix3f m;
		m << 0, -rHat(2), rHat(1),
		     rHat(2), 0, -rHat(0),
		     -rHat(1), rHat(0), 0;
		return m;
	}

	void createMatrix() {
		Vector3f r(x, y, z);
		float theta = r.norm() * M_PI / 180.0;
		Vector3f rHat = r / r.norm();
		Matrix3f rc = rCross(rHat);
		Matrix3f I;
		I << 1, 0, 0,
		     0, 1, 0,
		     0, 0, 1;
		Matrix3f matrix3 = I + sin(theta) * rc + (1- cos(theta)) * rc * rc ;
		
		matrix(0, 0) = matrix3(0, 0);
		matrix(1, 0) = matrix3(1, 0);
		matrix(2, 0) = matrix3(2, 0);
		
		matrix(0, 1) = matrix3(0, 1);
		matrix(1, 1) = matrix3(1, 1);
		matrix(2, 1) = matrix3(2, 1);

		matrix(0, 2) = matrix3(0, 2);
		matrix(1, 2) = matrix3(1, 2);
		matrix(2, 2) = matrix3(2, 2);

		matrix(0, 3) = 0;
		matrix(1, 3) = 0;
		matrix(2, 3) = 0;

		matrix(3, 0) = 0;
		matrix(3, 1) = 0;
		matrix(3, 2) = 0;
		matrix(3, 3) = 1;

		inverse = matrix.transpose();

	}

	rotation(float rx, float ry, float rz) {
		type = ROTATION;
		x = rx;
		y = ry;
		z = rz;
		createMatrix();
	}

	
};


struct color
{
	unsigned char r, g, b;

	color(int red, int green, int blue) {
		r = red;
		g = green;
		b = blue;
	}

	color() {
		r = 0;
		g = 0;
		b = 0;
	}

	color(Vector3f rgb) {
		r = rgb(0) * 255;
		g = rgb(1) * 255;
		b = rgb(2) * 255;
	}


	friend ostream& operator<<(ostream& os, const color& c) {
		os << (int)c.r << ", " << (int)c.g << ", " << (int)c.b;
		return os;
	}
};

struct material
{
	Vector3f diff, amb, spec, refl;
	float phongExp;

	material() {};

	material(float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, 
		     float specR, float specG, float specB, float phongExponent, float reflR, float reflG, float reflB) {

		diff << diffR, diffG, diffB;
		amb << ambR, ambG, ambB;
		spec << specR, specG, specB;
		refl << reflR, reflG, reflB;
		phongExp = phongExponent;
	}

};

// Objects : //////////////////////////////////////////////////////////////////////////////

struct object 
{
	int type;
	material mat;
	Matrix4f transformMatrix;
	Matrix4f inverseTransformMatrix;
};

struct sphere : object
{
	float radius;
	Vector3f center;

	sphere(float cx, float cy, float cz, float r, material m, Matrix4f transMatrix, Matrix4f invTransMatrix) {
		radius = r;
		center = Vector3f(cx, cy, cz);
		type = SPHERE;
		mat = m;
		translate initTrans(cx, cy, cz);
		scale initScale(r, r, r);
		transformMatrix = transMatrix * initScale * initTrans; // takes a sphere from obj space to world space
		inverseTransformMatrix = initTrans.inverse * initScale.inverse * invTransMatrix; // takes a sphere from world space to obj space
	}

};

struct triangle : object
{
	Vector3f a, b, c;

	triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, material m, Matrix4f transMatrix, Matrix4f invTransMatrix) {
		a = Vector3f(ax, ay, az);
		b = Vector3f(bx, by, bz);
		c = Vector3f(cx, cy, cz);
		type = TRIANGLE;
		mat = m;
		transformMatrix = transMatrix;
		inverseTransformMatrix = invTransMatrix;
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

	Vector4f p(Vector4f eye, Vector4f sMinE, float t) {
		return eye + t * sMinE;
	}

	// OLD:
	bool intersect(sphere sph, Vector3f* point) {
		// Check Discriminant:
		// A = sMinusE . sMinusE
		float A = sMinusE.dot(sMinusE);
		// B = 2 * sMinusE . (e - c)
		float B = 2 * sMinusE .dot(( e - sph.center ));
		// C = (e - c) . (e - c) - r^2
		float C = ( e - sph.center ).dot( ( e - sph.center ) ) - ( sph.radius * sph.radius );
		float discriminant = B*B - 4*A*C;
		if (discriminant >= 0) {
			// Use positive or negative value of discriminant depending on which results in the smallest t.
			float t;
			if(B < 0) {
				t = (-B - sqrt(discriminant)) / (2 * A);
			}
			else {
				t = (-B + sqrt(discriminant)) / (2 * A);
			}
			
			*point = p(t);
			return true;
		}
		return false;
	}

	// NEW:
	bool intersect(sphere sph, Vector3f* point, bool qqqq, float* tt) {
		// Check Discriminant:
		// A = sMinusE . sMinusE
		Matrix4f trans = sph.inverseTransformMatrix;
		Matrix4f inv = sph.transformMatrix;
		Vector4f eObj(e(0), e(1), e(2), 1);
		Vector4f sMinusEObj(sMinusE(0), sMinusE(1), sMinusE(2), 1);
		eObj = trans * eObj;
		sMinusEObj = trans * sMinusEObj;
		eObj(3) = 0;
		sMinusEObj(3) = 0;
		Vector4f centerObj(0, 0, 0, 1);

		/*Vector3f ne(eObj(0), eObj(1), eObj(2));
		Vector3f ns(sMinusEObj(0), sMinusEObj(1), sMinusEObj(2));
		ray nray(ne, ns + ne);
		Matrix4f ii;
		material iim;
		sphere nsph(0, 0, 0, 1, iim, ii, ii );
		return nray.intersect(nsph, point);*/

		float A = sMinusEObj.dot(sMinusEObj);
		// B = 2 * sMinusEObj . (eObj - c)
		float B = 2 * sMinusEObj .dot(( eObj - centerObj ));
		// C = (eObj - c) . (eObj - c) - r^2
		float C = ( eObj - centerObj ).dot( ( eObj - centerObj ) ) - 1;
		float discriminant = B*B - 4*A*C;
		if (discriminant >= 0) {
			// Only use negative value of discriminant, as this will be closer to the plane
			float t;
			if(B < 0) {
				t = (-B - sqrt(discriminant)) / (2 * A);
			}
			else {
				t = (-B + sqrt(discriminant)) / (2 * A);
			}
			/*Vector4f pointInWorldSpace = inv * p(eObj, sMinusEObj, t);
			(*point)(0) = pointInWorldSpace(0); // transform back into world coordinates
			(*point)(1) = pointInWorldSpace(1);
			(*point)(2) = pointInWorldSpace(2);*/
			*point = p(t);
			*tt = t;
			return true;
		}
		return false;
	}

	bool intersect(sphere sph, float* t) {
		// Check Discriminant:
		// A = sMinusE . sMinusE
		float A = sMinusE.dot(sMinusE);
		// B = 2 * sMinusE . (e - c)
		float B = 2 * sMinusE .dot(( e - sph.center ));
		// C = (e - c) . (e - c) - r^2
		float C = ( e - sph.center ).dot( ( e - sph.center ) ) - ( sph.radius * sph.radius );
		float discriminant = B*B - 4*A*C;
		if (discriminant >= 0) {
			// Only use negative value of discriminant, as this will be closer to the plane
			*t = (-B - sqrt(discriminant)) / (2 * A);
			return true;
		}
		return false;
	}

	bool intersect(triangle tri, Vector3f* point) {
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
		Vector3f B;
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
	}

	bool intersect(triangle tri, float* t) {
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
		Vector3f B;
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
		*t = (A(2, 1) * akjb + A(1, 1) * jcal + A(0, 1) * blkc) / M;
		return true;
	}

	
	
};

struct light
{
	float r, g, b;

	Vector3f l() {
		Vector3f v(r, g, b);
		return v;
	}
};

struct pointLight : light 
{
	Vector3f point;
	float falloff;

	pointLight(Vector3f p, float fall, Vector3f rgb) {
		point = p;
		falloff = fall;
		r = rgb(0);
		g = rgb(1);
		b = rgb(2);
	}
};

struct directionalLight : light
{
	Vector3f direction;

	directionalLight(Vector3f d, Vector3f rgb) {
		direction = d;
		r = rgb(0);
		g = rgb(1);
		b = rgb(2);
	}
};

struct ambientLight : light {  // this exists purely for naming convenience
	ambientLight(Vector3f rgb) {
		r = rgb(0);
		g = rgb(1);
		b = rgb(2);
	}
};





#endif  /* RAY_TRACER_OBJECTS_H */



