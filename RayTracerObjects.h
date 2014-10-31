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

//const float EPSILON = 0.025f;
const float EPSILON = 0.1f;

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
	Matrix4f objToWorld;
	Matrix4f worldToObj;
};

struct sphere : object
{
	float radius;
	Vector3f center;

	sphere(float cx, float cy, float cz, float r, material m, Matrix4f objectToWorld, Matrix4f worldToObject) {
		radius = r;
		Vector4f tmpCenter(cx, cy, cz, 1);
		tmpCenter = objectToWorld * tmpCenter;
		center = Vector3f(tmpCenter(0), tmpCenter(1), tmpCenter(2));
		type = SPHERE;
		mat = m;
		translate initTrans(cx, cy, cz);
		scale initScale(r, r, r);
		objToWorld =   objectToWorld * initTrans * initScale ; // takes a sphere from obj space to world space
		worldToObj = initScale.inverse * initTrans.inverse * worldToObject; // takes a sphere from world space to obj space
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
		objToWorld = transMatrix;
		worldToObj = invTransMatrix;
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
		sMinusE = sMinusE / sMinusE.norm(); // normalize the direction
	}

	ray(Vector3f start, Vector3f direction, bool differentiator) {
		e = start;
		sMinusE = direction / direction.norm();
	}

	Vector3f p(float t) {
		return e + t * (sMinusE);
	}

	Vector4f p(Vector4f eye, Vector4f sMinE, float t) {
		eye(3) = 1;
		sMinE(3) = 0;
		return eye + t * sMinE;
	}

	// OLD:
	bool intersect(sphere sph, Vector3f* point, bool qqq) {
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
	bool intersect(sphere sph, Vector3f* point, float* tempT) {
		// Check Discriminant:
		// A = sMinusE . sMinusE
		Matrix4f trans = sph.worldToObj;
		Matrix4f inv = sph.objToWorld;
		Vector4f eObj(e(0), e(1), e(2), 1); // eye in homogenized coords
		Vector4f sMinusEObj(sMinusE(0), sMinusE(1), sMinusE(2), 0); // sMinusE in homogenized coords, last val is 0 as it is a scalar
		eObj = trans * eObj; // take ray to obj space
		sMinusEObj = trans * sMinusEObj; // take ray to obj space
		eObj(3) = 0;
		sMinusEObj(3) = 0;

		float A = sMinusEObj.dot(sMinusEObj);
		// B = 2 * sMinusEObj . (eObj - c)
		float B = 2 * sMinusEObj .dot(( eObj ));
		// C = (eObj - c) . (eObj - c) - r^2
		float C = ( eObj ).dot( ( eObj ) ) - 1;
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

			// BIAS
			if(t < EPSILON) {
				return false;
			}

			// REMOVE THIS:
			*tempT = t;

			eObj(3) = 1;
			Vector4f pointInWorldSpace = inv * ( eObj + t * sMinusEObj); // take point to world space

			(*point)(0) = pointInWorldSpace(0); // transform back into world coordinates
			(*point)(1) = pointInWorldSpace(1);
			(*point)(2) = pointInWorldSpace(2);

			return true;
		}
		return false;
	}

	// OLD
	bool intersect(sphere sph, float* t, bool temptemp) {
		// Check Discriminant:
		// A = sMinusE . sMinusE
		float A = sMinusE.dot(sMinusE);
		// B = 2 * sMinusE . (e - c)
		float B = 2 * sMinusE .dot(( e - sph.center ));
		// C = (e - c) . (e - c) - r^2
		float C = ( e - sph.center ).dot( ( e - sph.center ) ) - ( sph.radius * sph.radius );
		float discriminant = B*B - 4*A*C;
		if (discriminant >= 0) {
			if(B < 0) {
				*t = (-B - sqrt(discriminant)) / (2 * A);
			}
			else {
				*t = (-B + sqrt(discriminant)) / (2 * A);
			}
			return true;
		}
		return false;
	}

	bool intersect(sphere sph) {
		// Check Discriminant:
		// A = sMinusE . sMinusE
		Matrix4f trans = sph.worldToObj;
		Matrix4f inv = sph.objToWorld;
		Vector4f eObj(e(0), e(1), e(2), 1); // eye in homogenized coords
		Vector4f sMinusEObj(sMinusE(0), sMinusE(1), sMinusE(2), 0); // sMinusE in homogenized coords, last val is 0 as it is a scalar
		eObj = trans * eObj; // take ray to obj space
		sMinusEObj = trans * sMinusEObj; // take ray to obj space
		eObj(3) = 0;
		sMinusEObj(3) = 0;

		float A = sMinusEObj.dot(sMinusEObj);
		// B = 2 * sMinusEObj . (eObj - c)
		float B = 2 * sMinusEObj .dot(( eObj ));
		// C = (eObj - c) . (eObj - c) - r^2
		float C = ( eObj ).dot( ( eObj ) ) - 1;
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

			// BIAS
			if(t < EPSILON) {
				return false;
			}

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



