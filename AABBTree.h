#include <iostream>
#include "Eigen/Dense"
#include <cmath>
#include "RayTracerObjects.h"

using namespace std;

const int AABB_SPHERE = 1;
const int AABB_TRI = 2;

struct box
{
	sphere* sph;
	triangle* tri:
	int type;

	box(sphere* s) {
		type = AABB_SPHERE;
		sph = s;
	}

	box(triangle t) {
		type = AABB_TRI;
		tri = t;
	}

	object contains() {
		return 
	}

};

struct boundingBox
{
	float minX, maxX, minY, maxY, minZ, maxZ;
	bool final;

	boundingBox(sphere sph) {
		float center = sph.c;
		float radius = sph.radius;
		minX = min(center(0) + radius, center(0) - radius);
		maxX = max(center(0) + radius, center(0) - radius);

		minY = min(center(1) + radius, center(1) - radius);
		maxY = max(center(1) + radius, center(1) - radius);

		minZ = min(center(2) + radius, center(2) - radius);
		maxZ = max(center(0) + radius, center(2) - radius);
	}

	boundingBox(triangle tri) {
		Vector3f v1 = tri->a;
		Vector3f v2 = tri->b;
		Vector3f v3 = tri->c;
		minX = min(v1(0), v2(0));
		minX = min(minX, v3(0));
		maxX = max(v1(0), v2(0));
		maxX = max(maxX, v3(0));

		minY = min(v1(1), v2(1));
		minY = min(minY, v3(1));
		maxY = max(v1(1), v2(1));
		maxY = max(maxY, v3(1));

		minZ = min(v1(2), v2(2));
		minZ = min(minZ, v3(2));
		maxZ = max(v1(2), v2(2));
		maxZ = max(maxZ, v3(2));
	}


	bool setIntersect(float x1, float x2, float y1, float y2, float z1, float z2) {
		float upper = min(x2, y2);
		// x1 < y1 < x2
		if(y1 >= x1 && y1 <= x2) {
			if(z1 >= y1 && z1 <= upper) {
				return true;
			}
		}
		// y1 < x1 < y2 <  x2
		else if(y2 >= x1 && y2 <= x2) {
			if(z1 >= y2 && z2 <= upper) {
				return true;
			}
		}
		return false;
	}


	bool hit(ray* r, hitResult* h) {
		float xe = r->e(0);
		float ye = r->e(1);
		float ze = r->e(2);

		float xd = r->direction(0);
		float yd = r->direction(1);
		float zd = r->direction(2);

		float txMin, txMax, tyMin, tyMax, tzMin, tzMax;
		float ax = 1 / xd;
		float ay = 1 / yd;
		float az = 1 / zd;
		if(ax >= 0) {
			txMin = (minX - xe) / xd;
			txMax = (maxX - xe) / xd;
		}
		else {
			txMin = (maxX - xe) / xd;
			txMax = (minX - xe) / xd;
		}
		if(ay >= 0) {
			tyMin = (minY - ye) / yd;
			tyMax = (maxY - ye) / yd;
		}
		else {
			tyMin = (maxY - ye) / yd;
			tyMax = (minY - ye) / yd;
		}
		if(az >= 0) {
			tzMin = (minX - ze) / zd;
			tzMax = (maxX - ze) / zd;
		}
		else {
			tzMin = (maxZ - ze) / zd;
			tzMax = (minZ - ze) / zd;
		}

		bool result = setIntersect(txMin, txMax, tyMin, tyMax, tzMin, tzMax);

	}

	bool in(boundingBox* b) {
		if(minX >= b->minX && minY >= b->minY && minZ >= b->minZ) {
			if(maxX <= b->maxX && )
		}
	}
};

struct aabbNode
{
	object* obj;

	aabbNode(object* o) {
		obj* = o;
	}

	object* contains() {
		return obj;
	}
};
