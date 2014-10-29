
#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string.h>
#include <cmath>

#include "Sampler.h"
#include "Canvas.h"
#include "RayTracer.h"
#include "RayTracerObjects.h"

using namespace std;


vector<sphere> spheres;
vector<triangle> triangles;
vector<pointLight> pointLights;
vector<directionalLight> directionalLights;
vector<ambientLight> ambientLights;

Vector3f eye;
// Coordinates of the image plane
Vector3f LL, LR, UL, UR;





// Read Arguments and do something with them
void parseArgs(char* filename) {

	const int MAX_CHARS_PER_LINE = 512;
	const int MAX_TOKENS_PER_LINE = 20;
	const char* const DELIMITER = " ";

	// create a file-reading object
  	ifstream fin;
  	fin.open("input.txt"); // open a file

  	material mat;
  	Matrix4f cumulativeTransform;
  	Matrix4f inverseCumulativeTransform;
  	Matrix4f I;
  	I << 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1;
	cumulativeTransform = I;
	inverseCumulativeTransform = I;
  	
  	// read each line of the file
  	while (!fin.eof())
  	{
  	  	// read an entire line into memory
  	  	char buf[MAX_CHARS_PER_LINE];
  	  	fin.getline(buf, MAX_CHARS_PER_LINE);
  	  	
  	  	// parse the line into blank-delimited tokens
  	  	int n = 0; // a for-loop index
  	  	
  	  	// array to store memory addresses of the tokens in buf
  	  	const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
  	  	
  	  	// parse the line
  	  	token[0] = strtok(buf, DELIMITER); // first token
  	  	if (token[0]) // zero if line is blank
  	  	{
  	  	  	for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
  	  	  	{
  	  	  	  	token[n] = strtok(0, DELIMITER); // subsequent tokens
  	  	  	  	if (!token[n]) break; // no more tokens
  	  	  	}
  	  	}
		

  	  	if(strcmp(token[0], "cam") == 0) {
  	  		eye << atof(token[1]), atof(token[2]), atof(token[3]);
  	  		LL << atof(token[4]), atof(token[5]), atof(token[6]);
  	  		LR << atof(token[7]), atof(token[8]), atof(token[9]);
  	  		UL << atof(token[10]), atof(token[11]), atof(token[12]);
  	  		UR << atof(token[13]), atof(token[14]), atof(token[15]);
  	  	}
  	  	else if(strcmp(token[0], "sph") == 0) {
  	  		sphere sph(atof(token[1]), atof(token[2]), atof(token[3]), atof(token[4]), mat, cumulativeTransform, inverseCumulativeTransform);
  	  		spheres.push_back(sph);
  	  	}
  	  	else if(strcmp(token[0], "tri") == 0) {
  	  		triangle tri(atof(token[1]), atof(token[2]), atof(token[3]), atof(token[4]), atof(token[5]), atof(token[6]), atof(token[7]), atof(token[8]), atof(token[9]), mat, cumulativeTransform, inverseCumulativeTransform);
  	  		triangles.push_back(tri);
  	  	}
  	  	else if(strcmp(token[0], "ltp") == 0) {
  	  		Vector3f point(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		Vector3f rgb(atof(token[4]), atof(token[5]), atof(token[6]));
  	  		int falloff = 0;
  	  		if (token[7]) {
  	  			falloff = atof(token[7]);
  	  		}
  	  		pointLight pl(point, falloff, rgb);
  	  		pointLights.push_back(pl);
  	  	}
  	  	else if(strcmp(token[0], "ltd") == 0) {
  	  		Vector3f dir(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		Vector3f rgb(atof(token[4]), atof(token[5]), atof(token[6]));
  	  		directionalLight dl(dir, rgb);
  	  		directionalLights.push_back(dl);
  	  	}
  	  	else if(strcmp(token[0], "lta") == 0) {
  	  		Vector3f rgb(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		ambientLight al(rgb);
  	  		ambientLights.push_back(al);
  	  	}
  	  	else if(strcmp(token[0], "mat") == 0) {
  	  		material mm(atof(token[1]), atof(token[2]), atof(token[3]), 
  	  					atof(token[4]), atof(token[5]), atof(token[6]), 
  	  					atof(token[7]), atof(token[8]), atof(token[9]), atof(token[10]),
  	  					atof(token[11]), atof(token[12]), atof(token[13]));
  	  		mat = mm;
  	  	}
  	  	else if(strcmp(token[0], "xft") == 0) {
  	  		translate tMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		cumulativeTransform = tMat * cumulativeTransform;
  	  		inverseCumulativeTransform = inverseCumulativeTransform * tMat.inverse;
  	  	}
  	  	else if(strcmp(token[0], "xfr") == 0) {
  	  		rotation rMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		cumulativeTransform = rMat * cumulativeTransform;
  	  		inverseCumulativeTransform = inverseCumulativeTransform * rMat.inverse;
  	  	}
  	  	else if(strcmp(token[0], "xfs") == 0) {
  	  		scale sMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		cumulativeTransform = sMat * cumulativeTransform;
  	  		inverseCumulativeTransform = inverseCumulativeTransform * sMat.inverse;
  	  	}
  	  	else if(strcmp(token[0], "xfz") == 0) {
  	  		cumulativeTransform = I;
  	  	}
  	  	else {
  	  		cout << "UNRECOGNIZED TYPE: " << token[0] << endl;
  	  	}

  	}


}

int main(int argc, char* argv[])
{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// General Purpose - renders input file

	/*int size = 400;

	char* inFile = "input.txt";
	parseArgs(inFile);*/


			/*Sampler s(size, size, LL, LR, UL, UR);
			Canvas c(size, size);
			sphere toTest = spheres[0];
			for(int i = 0; i < size; i++) {
				for(int j = 0; j < size; j++) {
					ray r(eye, s.getSample());
					Vector3f p1, p2;
					bool newm = r.intersect(toTest, &p1);
					bool oldm = r.intersect(toTest, &p2, true);
					if(newm && oldm) {
						if(p1 != p2) {
							cout << "NEW METHOD: " << endl << p1 << endl << endl;
							cout << "OLD METHOD: " << endl << p2 << endl << endl;
						}
					}
					else if(newm != oldm){
						cout << "MISMATCH, methods did not both return the same" << endl;
						cout << "NEW: " << newm << "	 OLD: " << oldm << endl; 
						return 0;
					}
				}
			}*/


	/*Sampler s(size, size, LL, LR, UL, UR);
	Canvas c(size, size);
	RayTracer rt(eye, spheres, triangles, pointLights, directionalLights, ambientLights);
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			c.addPixel(rt.trace(s.getSample()));
		}
	}
	c.encode("image.png");*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Test for moving between object and world space


	Vector3f e(0, 0, 0);
	Vector3f p(1, 1, 1);

	Matrix4f I;
	I << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;

	Matrix3f I3;
	I3 << 1, 0, 0,
		     0, 1, 0,
		     0, 0, 1;

	ray r(e, p);
	material mm;
	sphere sph(20, 50, 50, 10, mm, I, I);
	Matrix4f trans = sph.worldToObj;
	Matrix4f inv = sph.objToWorld;
	Vector3f pp(-100, -100, -100);
	cout << "TRANSFORM FOR SPHERE: " << endl;
	cout << sph.worldToObj << endl << endl;
	cout << "INVERSE FOR SPHERE: " << endl;
	cout << sph.objToWorld << endl << endl;
	float tcheck;
	if(r.intersect(sph, &pp)) {
		cout << "INTERSECT" << endl;
		cout << "e :" << endl << r.e << endl << endl;
		cout << "sminuse :" << endl << r.sMinusE << endl << endl;
		cout << "POINT : " << endl << pp << endl << endl;
		cout << inv * trans << endl << endl;
		bool cq = r.intersect(sph, &pp, true);
		cout << "OLD POINT : " << endl << pp << endl << endl;
	}
	else {
		cout << "NOOOOOOOOOOOOOOOO" << endl;
		/*Vector4f ee(0, 0, 0, 1);
		Vector4f ss(r.sMinusE(0), r.sMinusE(1), r.sMinusE(2), 1);
		ee = trans * ee;	
		ss = trans * ss;
		cout << ee << endl << endl;
		cout << ss << endl << endl;
		Vector3f ne(ee(0), ee(1), ee(2));
		Vector3f np(ss(0), ss(1), ss(2));
		ray newr(ne, np + ne);
		sphere newsphsph(0, 0, 0, 1, mm, I, I);
		cout << "new ray: " << endl << endl << newr.e << endl << endl << newr.sMinusE << endl << endl;
		cout << "intersect = " << newr.intersect(newsphsph, &pp) << endl << endl;
		cout << pp << endl << endl;
		Vector4f ppp(pp(0), pp(1), pp(2), 1);
		cout << inv * ppp << endl;*/
	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Transform test:


/*	Vector4f vv(1, 1, 1, 1);
	translate trans(3, 4, 5);
	scale sc(1, 2, 3);
	rotation rr(0, 180, 0);
	cout << "Transform MAtrix" << endl;
	cout << trans.matrix << endl << endl;
	cout << "operator test:" << endl;
	cout << trans.matrix * vv << endl << endl;
	cout << "Scale MAtrix" << endl;
	cout << sc.matrix << endl << endl;
	cout << "operator test:" << endl;
	cout << sc.matrix * vv << endl << endl;
	cout << "Rotation MAtrix" << endl;
	cout << rr.matrix << endl << endl;
	cout << "operator test:" << endl;
	cout << rr.matrix * vv << endl << endl;

	cout << "combined operations:" << endl;
	Vector4f q(3, 3, 3, 1);
	translate qq(-1, -1, -1);
	scale qqq(2, 2, 2);
	rotation qqqq(0, 180, 0);
	cout << qqqq * qqq * qq * q<< endl;*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Single sphere for shading:


	/*int size = 100;
	Vector3f eye(0, 0, 100);
	Sampler s(size, size, -50, 50,  50, 50,   50, -50, -50, -50);
	Canvas c(size, size);
	// amb, diffuse, spec
	material mat1(0.1f, 0.1f, 0, 
				  1, 1, 0, 
				  0.8, 0.8, 0.8);

	Vector3f pldir(200, 200, 200);
	Vector3f plrgb(0.6, 0.6, 0.6);
	pointLight ppll(pldir, 1,  plrgb);
	pointLights.push_back(ppll);
	sphere sph1(0, 0, -50, 45, mat1);
	spheres.push_back(sph1);
	RayTracer rt(eye, spheres, triangles, pointLights, directionalLights);
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			c.addPixel(rt.trace(s.getSample()));
		}
	}
	c.encode("image.png");*/




/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// spheres and triangles on each corner to test alignment

	/*int size = 100;
	Sampler s(size, size, -size/2, size/2, size/2, size/2, size/2, -size/2, -size/2, -size/2);
	Canvas c(size, size);
	// amb = (0.6, 0.6, 0.6), diff = (0, 0.8, 0.1) spec = (0, 0, 0)
	material mat1(0.6f, 0.6f, 0.6f, 0, 0.8f, 0.1f, 0, 0, 0);
	Vector3f eye(0, 0, -100);
	sphere sph1(-21, 21, 0, 10, mat1); // top left, smaller
	sphere sph2(21, -21, 0, 20, mat1); // bottom right, larger
	triangle tri1(21, 21, 0, 21, 15, 0, 10, 18, 0, mat1); // top right, longer
	triangle tri2(-21, -21, 0, -21, -15, 0, -15, -18, 0, mat1); // bottom left, shorter
	Vector3f plpoint(0, 0, 0);
	Vector3f plcolor(1, 1, 1);
	pointLight ppll(plpoint, 1, plcolor);
	spheres.push_back(sph1);
	spheres.push_back(sph2);
	triangles.push_back(tri1);
	triangles.push_back(tri2);
	pointLights.push_back(ppll);
	RayTracer rt(eye, spheres, triangles, pointLights, directionalLights);
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			c.addPixel(rt.trace(s.getSample()));
		}
	}
	c.encode("image.png");*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Single sphere, centre

	/*int size = 100;
	Sampler s(size, size, -size/2, size/2, size/2, size/2, size/2, -size/2, -size/2, -size/2);
	Canvas c(size, size);
	Vector3f eye(0, 0, -100);
	sphere sph1(0, 0, 0, 20);
	spheres.push_back(sph1);
	RayTracer rt(eye, spheres, spheres.size(), triangles, triangles.size());
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			c.addPixel(rt.trace(s.getSample()));
		}
	}
	c.encode("image.png");*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////






	
	/*cout << " ------ Input Parse Test ------ " << endl;
	parseArgs(inFile);
	cout << endl;*/

	return 0;
}

