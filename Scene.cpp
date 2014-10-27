
#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string>

#include "Sampler.h"
#include "Canvas.h"
#include "RayTracer.h"
#include "RayTracerObjects.h"

using namespace std;


vector<sphere> spheres;
vector<triangle> triangles;
vector<pointLight> pointLights;
vector<directionalLight> directionalLights;


// Read Arguments and do something with them
void parseArgs(char* filename) {
	fstream f(filename);
	string line;
	if(f.is_open()) {
		while(getline(f, line)) {
			cout << line << endl;
			// if sphere: spheres.push_back(sphere(x, y, z))
			// if triangle: triangles.push_back(triangle(x, y, z))
		}
		f.close();
	}
	else {
		cout << "Unable to open file " << filename << endl;
	}

}

int main(int argc, char* argv[])
{

	Vector3f vv(1, 1, 1);
	cout << vv.norm() << endl;
	cout << vv / vv.norm() << endl << endl;

	Vector3f vv1(2, 2, 2);
	cout << vv1 / vv1.norm() << endl << endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// spheres and triangles on each corner to test alignment

	/*int size = 100;
	Sampler s(size, size, -size/2, size/2, size/2, size/2, size/2, -size/2, -size/2, -size/2);
	Canvas c(size, size);
	Vector3f eye(0, 0, -100);
	sphere sph1(-21, 21, 0, 10); // top left, smaller
	sphere sph2(21, -21, 0, 20); // bottom right, larger
	triangle tri1(21, 21, 0, 21, 15, 0, 10, 18, 0); // top right, longer
	triangle tri2(-21, -21, 0, -21, -15, 0, -15, -18, 0); // bottom left, shorter
	spheres.push_back(sph1);
	spheres.push_back(sph2);
	triangles.push_back(tri1);
	triangles.push_back(tri2);
	RayTracer rt(eye, spheres, spheres.size(), triangles, triangles.size());
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

