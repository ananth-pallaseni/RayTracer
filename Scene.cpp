
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
  	Matrix4f worldToObj;
  	Matrix4f objToWorld;
  	Matrix4f I;
  	I << 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1;

	worldToObj = I;
  	objToWorld = I;
  	
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
  	  		sphere sph(atof(token[1]), atof(token[2]), atof(token[3]), atof(token[4]), mat, objToWorld, worldToObj);
  	  		spheres.push_back(sph);
  	  	}
  	  	else if(strcmp(token[0], "tri") == 0) {
  	  		triangle tri(atof(token[1]), atof(token[2]), atof(token[3]), atof(token[4]), atof(token[5]), atof(token[6]), atof(token[7]), atof(token[8]), atof(token[9]), mat, objToWorld, worldToObj);
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
  	  		worldToObj = tMat.inverse * worldToObj;
  			objToWorld = objToWorld * tMat;

  	  	}
  	  	else if(strcmp(token[0], "xfr") == 0) {
  	  		rotation rMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		worldToObj = rMat.inverse * worldToObj;
  			objToWorld = objToWorld * rMat;
  	  	}
  	  	else if(strcmp(token[0], "xfs") == 0) {
  	  		scale sMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		worldToObj = sMat.inverse * worldToObj;
  			objToWorld = objToWorld * sMat;
  	  	}
  	  	else if(strcmp(token[0], "xfz") == 0) {
  	  		worldToObj = I;
  			objToWorld = I;
  	  	}
  	  	else {
  	  		cout << "UNRECOGNIZED TYPE: " << token[0] << endl;
  	  	}

  	}


}

Vector3f unit11(Vector3f v) {
	return v / v.norm();
}

Vector3f sphereNormal11(Vector3f pointOnShape, sphere shape) {
	return unit11(pointOnShape - shape.center);
}


int main(int argc, char* argv[])
{


int size = 600;

	char* inFile = "input.txt";
	parseArgs(inFile);


	Sampler s(size, size, LL, LR, UL, UR);
	Canvas c(size, size);
	RayTracer rt(eye, spheres, triangles, pointLights, directionalLights, ambientLights);
	// sph[0] is at -40, 0, -100 with radius 30
	Vector3f start(-18.7867966, 0, -78.7867966);
	Vector3f n = sphereNormal11(start, spheres[0]);
	cout << "NORMAL: " << endl << n << endl << endl;
	Vector3f eyeStart(-18.7867966, 0, 0);
	ray inc(eyeStart, start);
	cout << "INC: " << endl << inc.sMinusE << endl << endl;
	ray refl(start, inc.sMinusE - 2 * n * (inc.sMinusE.dot(n)));
	cout << "INTERMEDIATE CALC: " << inc.sMinusE(0) << " - " << (2 * n * (inc.sMinusE.dot(n)))(0) << endl;
	cout << "INTERMEDIATE CALC: " << inc.sMinusE(1) << " - " << (2 * n * (inc.sMinusE.dot(n)))(1) << endl;
	cout << "INTERMEDIATE CALC: " << inc.sMinusE(2) << " - " << (2 * n * (inc.sMinusE.dot(n)))(2) << endl;
	cout << "REFL: " << endl << refl.sMinusE << endl << endl;
	Vector3f inters;
	float t;
	if(refl.intersect(spheres[1], &inters, &t)) {
		cout << "HIT" << endl << inters << endl << endl;
	}
	else {
		cout << "NOO" << endl << endl;
	}
	//cout << spheres[0].center << endl << endl;





/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// General Purpose - renders input file

	/*int size = 600;

	char* inFile = "input.txt";
	parseArgs(inFile);


	Sampler s(size, size, LL, LR, UL, UR);
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


	/*Vector3f e(0, 0, 0);
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
	sphere sph(50, 50, 50, 10, mm, I, I);
	Matrix4f trans = sph.worldToObj;
	Matrix4f inv = sph.objToWorld;
	Vector3f pp(-100, -100, -100);
	Vector3f ppp(-200, -200, -200);
	cout << "WORLD TO OBJ: " << endl;
	cout << sph.worldToObj << endl << endl;
	cout << "OBJ TO WORLD: " << endl;
	cout << sph.objToWorld << endl << endl;
	cout << "?????????????????????? BEGIN INSIDE NEW INTERSECT ??????????????????????????????" << endl << endl;
	bool test1 = intersect(sph, &pp, e, r.sMinusE);
	cout << "???????????????????????  END INSIDE NEW INTERSECT ??????????????????????????????" << endl << endl;
	cout << "?????????????????????? BEGIN INSIDE OLD INTERSECT ??????????????????????????????" << endl << endl;
	bool test2 = intersect(sph, &ppp, true, e, r.sMinusE);
	cout << "???????????????????????  END INSIDE OLD INTERSECT ??????????????????????????????" << endl << endl;
	cout << "POINT, NEW: " << endl << pp << endl << endl;
	cout << "POINT, OLD: " << endl << ppp << endl << endl;
	*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



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

