
#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string.h>
#include <cmath>
#include <ctime>

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
void parseObj(char* filename, material mat, Matrix4f objToWorld, Matrix4f worldToObj) {

  const int MAX_CHARS_PER_LINE = 512;
  const int MAX_TOKENS_PER_LINE = 20;
  const char* const DELIMITER = " ";

  // create a file-reading object
    ifstream fin;
    fin.open("scene12.obj"); // open a file

    vector<Vector3f> vertices;
    Vector3f first;
    vertices.push_back(first);

    cout << "1" << endl;
    
    // read each line of the file
    while (!fin.eof())
    {
    	cout << "2" << endl;
        // read an entire line into memory
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the tokens in buf
        const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
        
        cout << "3" << endl;

        // parse the line
        token[0] = strtok(buf, DELIMITER); // first token
        if (token[0]) // zero if line is blank
        {
        	cout << "4" << endl;
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER); // subsequent tokens
                if (!token[n]) break; // no more tokens
            }
        }
    

        if(strcmp(token[0], "v") == 0) {
          Vector3f vertex(atof(token[1]), atof(token[2]), atof(token[3]));
          vertices.push_back(vertex); // assuming push back adds to end
          cout << vertex << endl << endl << endl;
        }
        else if(strcmp(token[0], "f") == 0) {
          Vector3f v1 = vertices[atoi(token[1])];
          Vector3f v2 = vertices[atoi(token[2])];
          Vector3f v3 = vertices[atoi(token[3])];
          cout << v1 << endl << endl << v2 << endl << endl << v3 << endl << endl << endl;
          triangle tri(v1(0), v1(1), v1(2), v2(0), v2(1), v2(2), v3(0), v3(1), v3(2), mat, objToWorld);
          triangles.push_back(tri);
        }
        
        else {
          cout << "UNRECOGNIZED TYPE: " << token[0] << endl;
        }

    }


}



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
  	Matrix4f inOrder;
  	Matrix4f I;
  	I << 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1;

	worldToObj = I;
  	objToWorld = I;
  	inOrder = I;
  	
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
  	  		triangle tri(atof(token[1]), atof(token[2]), atof(token[3]), atof(token[4]), atof(token[5]), atof(token[6]), atof(token[7]), atof(token[8]), atof(token[9]), mat, objToWorld);
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
  			inOrder = tMat * inOrder;

  	  	}
  	  	else if(strcmp(token[0], "xfr") == 0) {
  	  		rotation rMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		worldToObj = rMat.inverse * worldToObj;
  			objToWorld = objToWorld * rMat;
  			inOrder = rMat * inOrder;
  	  	}
  	  	else if(strcmp(token[0], "xfs") == 0) {
  	  		scale sMat(atof(token[1]), atof(token[2]), atof(token[3]));
  	  		worldToObj = sMat.inverse * worldToObj;
  			objToWorld = objToWorld * sMat;
  			inOrder = sMat * inOrder;
  	  	}
  	  	else if(strcmp(token[0], "xfz") == 0) {
  	  		worldToObj = I;
  			objToWorld = I;
  			inOrder = I;
  	  	}
  	  	else {
  	  		cout << "UNRECOGNIZED TYPE: " << token[0] << endl;
  	  	}

  	}


}

Vector3f p11(float t, Vector3f sMinusE, Vector3f e) {
		return e + t * (sMinusE);
	}

bool intersect11(triangle tri, Vector3f* point, Vector3f sMinusE, Vector3f e) {
		cout << "tri a: " << endl	 << tri.a << endl << endl;
		cout << "tri b: " << endl	 << tri.b << endl << endl;
		cout << "tri c: " << endl	 << tri.c << endl << endl;
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
		cout << "A: " << endl << A << endl << endl;

		// Setri up matririx B:
		Vector3f B;
		B(0) = tri.a(0) - sMinusE(0);
		B(1) = tri.a(1) - sMinusE(1);
		B(2) = tri.a(2) - sMinusE(2);
		cout << "B: " << endl << B << endl << endl;
		// Store these vals to save computation:
		float eihf = A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1);
		float gfdi = A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2);
		float dheg = A(0, 1) * A(1, 2) - A(1, 1) * A(0, 2);
		float akjb = A(0, 0) * B(1) - B(0) * A(1, 0);
		float jcal = B(0) * A(2, 0) - A(0, 0) * B(2);
		float blkc = A(1, 0) * B(2) - B(1) * A(2, 0);
		cout << "eihf: " << eihf << endl;
		cout << "gfdi: " << gfdi << endl;
		cout << "dheg: " << dheg << endl;
		cout << "akjb: " << akjb << endl;
		cout << "jcal: " << jcal << endl;
		cout << "blkc: " << blkc << endl;
		// By Cramers Rule:
		float M = A(0, 0) * eihf + A(1, 0) * gfdi + A(2, 0) * dheg;
		cout << "M: " << endl << M << endl << endl;
		float gamma = (A(2, 2) * akjb + A(1, 2) * jcal + A(0, 2) * blkc) / M;
		cout << "GAMMA: " << endl << gamma << endl << endl;
		if (gamma < 0 || gamma > 1) {
			return false;
		}
		float beta = (B(0) * eihf + B(1) * gfdi + B(2) * dheg) / M;
		cout << "BETA: " << endl << beta << endl << endl;
		if (beta < 0 || beta > 1 - gamma) {
			return false;
		}
		float t = (A(2, 1) * akjb + A(1, 1) * jcal + A(0, 1) * blkc) / M;
		cout << "T: " << endl << t << endl << endl;
		*point = p11(t, sMinusE, e);
		return true;
	}

	bool intersect22(triangle tri, Vector3f* point, Vector3f sMinusE, Vector3f e) {
		// check if it intersects a plane:
		Vector3f side1 = tri.a - tri.b;
		Vector3f side2 = tri.a - tri.c;
		Vector3f planeNormal = side1.cross(side2);
		planeNormal = planeNormal / planeNormal.norm();
		cout << "NORMAL TO PLANE: " << planeNormal << endl << endl;
		float numerator = planeNormal.dot(tri.a - e);
		float denominator = planeNormal.dot(sMinusE);
		cout << "NUMERATOR: " << numerator << endl;
		cout << "DENOMINATOR: " << denominator << endl;
		if(denominator == 0) {
			// ray is parallel to plane:
			return false;
		}
		float r1 = numerator / denominator;
		cout << "r1: " << r1 << endl;
		if(r1 < 0) {
			// means ray does not intersect plane:
			return false;
		}
		Vector3f pointToTest = p11(r1, sMinusE, e);
		cout << "POINT TO TEST: " << endl << pointToTest << endl << endl;

		// Now check if the point lies within the triangle:
		Vector3f u = tri.b - tri.a;
		Vector3f v = tri.c - tri.a;
		Vector3f w = pointToTest - tri.a;
		cout << "U: " << endl << u << endl << endl;
		cout << "V: " << endl << v << endl << endl;
		cout << "W: " << endl << w << endl << endl;

		float denom1 = u.dot(v) * u.dot(v) - u.dot(u) * v.dot(v);
		cout << "DENOM1: " << denom1 << endl;
		float s1 = u.dot(v) * w.dot(v) - v.dot(v) * w.dot(u);
		s1 = s1 / denom1;
		float t1 = u.dot(v) * w.dot(u) - u.dot(u) * w.dot(v);
		t1 = t1 / denom1;
		cout << "S1: " << s1 << endl;
		cout << "T1: " << t1 << endl;
		if(s1 < 0 || t1 < 0 || s1 + t1 > 1) {
			return false;
		}
		*point = pointToTest;
		return true;
	}

Vector3f triangleNormal11(Vector3f pointOnShape, triangle shape, ray r) {
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

int main(int argc, char* argv[])
{

	material mm;
	Matrix4f matr;
matr << 1, 0, 0, 0,
		0, 1, 0, 0, 
		0, 0, 1, 0,
		0, 0, 0, 1;

	char* name = "scene12.obj";
	parseObj(name, mm, matr, matr);

/*material mm;
Matrix4f matr;
matr << 1, 0, 0, 0,
		0, 1, 0, 0, 
		0, 0, 1, 0,
		0, 0, 0, 1;
triangle tri(0, -95, 0, -100, -95, 100, 100, -95, 100, mm, matr);
Vector3f eye(0, 75, 500);
Vector3f point(50, -95, 50);
ray r(eye, point);
Vector3f n = triangleNormal11(point, tri, r);

cout << "NORMAL: " << n(0) << ", " << n(1) << ", " << n(2) << endl;*/

/*Vector3f eye(0, 0, 150);
Vector3f point(0, 0, 0);
// ray should be in 0, 0, -1 direction
ray r(eye, point);
Vector3f p;
float t;
bool check = intersect22(tri, &p, r.sMinusE, r.e);
bool check2 = r.intersect(tri, &t);
if(check) {
	cout << "POINT: " << endl << p << endl;
	cout << "NORMAL: " << endl << triangleNormal11(p, tri, r) << endl;
	cout << "T: " << t << endl;
}
else {
	cout << "NO INTERSECTION" << endl;
}*/



/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// General Purpose - renders input file

	/*int size = 500;

	char* inFile = "input.txt";
	clock_t startTime;
	double duration;
	startTime = clock();

	parseArgs(inFile);

	Sampler s(size, size, LL, LR, UL, UR);
	Canvas c(size, size);
	RayTracer rt(eye, spheres, triangles, pointLights, directionalLights, ambientLights);
	for(int i = 0 ; i < size; i++) {
		for(int j = 0 ; j < size; j++) {
			c.addPixel(rt.trace(s.getSample()));
			
		}
	}
	c.encode("image.png");

	duration = (clock() - startTime) / (double) CLOCKS_PER_SEC;
	cout << "DONE" << endl;
	cout << "TIME: " << duration << " seconds" << endl;*/

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

