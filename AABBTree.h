#ifndef AABBTREE_H
#define AABBTREE_H

#include <iostream>
#include "Eigen/Dense"
#include <cmath>
#include "RayTracerObjects.h"

using namespace std;

const int AABB_BOX = 0;
const int AABB_SPHERE = 1;
const int AABB_TRI = 2;


struct boundingBox
{
	object* obj;
	float minX, maxX, minY, maxY, minZ, maxZ;
	bool leaf;
	int type;
	float volume;

	boundingBox* left;
	boundingBox* right;

	void volCalc() {
		volume = (maxX - minX) * (maxY - minY) * (maxZ - minZ);
	}

	boundingBox() {};

	boundingBox(boundingBox* b1, boundingBox* b2) {
		leaf = false;
		minX = min(b1->minX, b2->minX);
		minY = min(b1->minY, b2->minY);
		minZ = min(b1->minZ, b2->minZ);
		maxX = max(b1->maxX, b2->maxX);
		maxY = max(b1->maxY, b2->maxY);
		maxZ = max(b1->maxZ, b2->maxZ);
		volCalc();
		left = b1;
		right = b2;
		type = AABB_BOX;
		cout << endl << "CREATING BB" << endl;
		cout << "minX: " << minX << "   maxX: " << maxX << endl;
		cout << "minY: " << minY << "   maxY: " << maxY << endl;
		cout << "minZ: " << minZ << "   maxZ: " << maxZ << endl;
		cout << "END CREATE BB" << endl << endl;
	}

	boundingBox(sphere* sph) {
		cout << "Added sphere BB" << endl;
		leaf = true;
		Vector3f center = sph->center;
		float radius = sph->radius;
		minX = min(center(0) + radius, center(0) - radius);
		maxX = max(center(0) + radius, center(0) - radius);

		minY = min(center(1) + radius, center(1) - radius);
		maxY = max(center(1) + radius, center(1) - radius);

		minZ = min(center(2) + radius, center(2) - radius);
		maxZ = max(center(0) + radius, center(2) - radius);
		volCalc();
		obj = sph;
		type = AABB_SPHERE;
	}

	boundingBox(triangle* tri) {
		cout << "Added triangle bb" << endl;
		leaf = true;
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
		volCalc();
		obj = tri;
		type = AABB_TRI;
	}


	bool setIntersect(float x1, float x2, float y1, float y2, float z1, float z2) {
		// find interval x -> y:
		float i1, i2;
		if(x1 <= y1) {
			// x1 < y1
			if(y1 <= x2) {
				// x1 < y1 < x2 ... y2
				i1 = y1;
				i2 = min(x2, y2);
			}
			else {
				return false;
			}
		}
		else {
			// y1 < x1
			if(x1 <= y2) {
				i1 = x1;
				i2 = min(x2, y2);
			}
			else {
				return false;
			}

		}

		if(z1 <= i1) {
			if(i1 <= z2) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if(z1 <= i2) {
				return true;
			}
			else {
				return false;
			}
		}
	}


	bool hit(ray* r, hitResult* result) {
		cout << "CALL TO HIT" << endl;
		float xe = r->e(0);
		float ye = r->e(1);
		float ze = r->e(2);
		cout << "ASSIGNED EYE VALUES" << endl;
		float xd = r->direction(0);
		float yd = r->direction(1);
		float zd = r->direction(2);
		//cout << "minX: " << minX << "  XE: " << xe << endl; cout << "maxX: " << maxX << "  XE: " << xe << endl;
		//cout << "MINY: " << minY << "  YE: " << ye << endl; cout << "maxY: " << maxY << "  YE: " << ye << endl;
		//cout << "minZ: " << minZ << "  ZE: " << ze << endl; cout << "maxZ: " << maxZ << "  ZE: " << ze << endl;

		cout << "ADDIGNED DIRECTION VALUES" << endl;

		float txMin, txMax, tyMin, tyMax, tzMin, tzMax;
		float ax = 1 / xd; //cout << "xd: " << xd << endl; cout << "ax: " << ax << endl;
		float ay = 1 / yd; //cout << "yd: " << yd << endl; cout << "ay: " << ay << endl;
		float az = 1 / zd; //cout << "za: " << zd << endl; cout << "az: " << az << endl;

		cout << "CREATED AX, AY, AZ VALUES" << endl;
		if(ax >= 0) {
			cout << "AX MORE THAN 0 ,   ASSIGN TMINX AND TMAXX" << endl;
			txMin = (minX - xe) * ax;
			txMax = (maxX - xe) * ax;
		}
		else {
			cout << "AX LESS THAN 0 ,   ASSIGN TMINX AND TMAXX" << endl;
			cout << "maxX = " << endl;
			cout << maxX << " 		minX = " << minX << endl;
			cout << "ax: " << ax << endl;
			txMin = (maxX - xe) * ax;
			txMax = (minX - xe) * ax;
			cout << "HAVE ASSIGNED X VALUES" << endl;
		}
		if(ay >= 0) {
			cout << "ASSIGN TMINY AND TMAXY" << endl;
			tyMin = (minY - ye) * ay; 
			tyMax = (maxY - ye) * ay;
		}
		else {
			cout << "ASSIGN TMINY AND TMAXY" << endl;
			tyMin = (maxY - ye) * ay;
			tyMax = (minY - ye) * ay;
		}
		if(az >= 0) {
			cout << "ASSIGN TMINZ AND TMAXZ" << endl;
			tzMin = (minZ- ze) * az;
			tzMax = (maxZ- ze) * az;
		}
		else {
			cout << "ASSIGN TMINZ AND TMAXZ" << endl;
			tzMin = (maxZ - ze) * az;
			tzMax = (minZ - ze) * az;
		}

		cout << "END COMPARISONS" << endl;
		// checks for nans
		if(	!(txMin == txMin) ) {txMin = 0;};
		if(	!(txMax == txMax) ) {txMax = 0;};
		if(	!(tyMin == tyMin) ) {tyMin = 0;};
		if(	!(tyMax == tyMax) ) {tyMax = 0;};
		if(	!(tzMin == tzMin) ) {tzMin = 0;};
		if(	!(tzMax == tzMax) ) {tzMax = 0;};


		//cout << "Txmin : " << txMin << "   TxMax: " << txMax << endl;
		//cout << "Txyin : " << tyMin << "   TyMax: " << tyMax << endl;
		//cout << "Tzmin : " << tzMin << "   TzMax: " << tzMax << endl;
		

		if(leaf) {
			cout << "STILL IN HIT, IS A LEAF" << endl;
			switch (type) {
				case AABB_SPHERE:
					return r->intersect( *( (sphere*) obj ), result);

				case AABB_TRI:
					return r->intersect( *( (triangle*) obj ), result);
			}
		}
		else {
			cout << "STILL IN HIT, NOT A LEAF" << endl;
			return setIntersect(txMin, txMax, tyMin, tyMax, tzMin, tzMax);;
		}
	}

	bool rayTraverse(ray* r, hitResult* result) {
	  cout << "START TRAVERSE" << endl;
	  bool hitsBox = hit(r, result);
	  cout << "CHECKING IF HIT BOX: " << endl;
	  if(hitsBox) {
	    cout << "HIT" << endl;
	    if(leaf) {
	      cout << "IS A LEAF" << endl;
	      return true;
	    }
	    cout << "NOT A LEAF" << endl;
	    hitResult r1, r2;
	    bool c1 = left->rayTraverse(r,  &r1);
	    bool c2 = right->rayTraverse(r,  &r2);
	    if(c1 && c2) {
	      if(r1.t < r2.t) {
	        *result = r1;
	        return true;
	      }
	      *result = r2;
	      return true;
	    }
	    else if(c1) {
	      *result = r1;
	      return true;
	    }
	    else if(c2) {
	      *result = r2;
	      return true;
	    }
	  }
	  else {
	  	cout << "NO HIT" << endl;
	    return false;
	  }
	}


};


#endif /* AABBTREE_H */