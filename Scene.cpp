
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
int numSpheres;
int numTriangles;


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
	/*int size = 100;
	Sampler s(size, size, -size/2, size/2, size/2, size/2, size/2, -size/2, -size/2, -size/2);
	Canvas c(size, size);
	Vector3f eye(0, 0, -100);
	sphere sph1(-70, 70, 2, 10); // top left
	sphere sph2(70, -70, 2, 50); // bottom right
	triangle tri1(70, 70, 2, 70, 50, 2, 50, 60, 2); // top right
	triangle tri2(-70, -70, 2, -70, -50, 2, -50, -60, 2); // bottom left
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

	int size = 100;
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
	c.encode("image.png");



	// Intersect Tests
	/*Vector3f e(0, 0, 0);
	Vector3f s(1, 1, 0);
	ray r(e, s);
	sphere sph(3, 3, 0, 1);
	Vector3f result;
	if(r.intersect(sph, &result)) {
		cout << result << endl;
	}
	else {
		cout << "NO INTERSECT 1" << endl;
	}

	Vector3f e2(0, 1, 0);
	Vector3f s2(1, 1, 0);
	ray r2(e2, s2);
	sphere sph2( 3, 0, 0, 1);
	Vector3f result2;
	if(r2.intersect(sph2, &result2)) {
		cout << result2 << endl;
	}
	else {
		cout << "NO INTERSECT 2" << endl;
	}

	Vector3f e3(0, 0, 0);
	Vector3f s3(1, 1, 0);
	ray r3(e3, s3);
	sphere sph3(3, 3, 3, 1);
	Vector3f result3;
	if(!r.intersect(sph3, &result3)) {
	}
	else {
		cout << "FOUND INTERSECT WHERE THERE WAS NONE" << endl;
	}*/






	/*for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			c.addPixel(255 - i, 0, j);
		}
	}

	c.encode("image.png");*/


	/*cout << " ------ Input Parse Test ------ " << endl;
	parseArgs(inFile);
	cout << endl;*/

	// testing declarations	
	/*camera c(1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 8);
	cout << " ------ Camera Test ------" << endl << "Camera Coords: " << endl << c.e << endl ;
	cout << "Image Plane: " << endl << c.ll << endl << c.lr << endl << c.ul << endl << c.ur << endl;

	sphere sph(1, 2, 3, 4);
	cout << " ------ Sphere Test ------" << endl << "Sphere Coords: " << endl << sph.center << endl << "Sphere radius: " << sph.radius << endl;

	triangle t(1, 2, 3, 4, 5, 6, 7, 8, 9);
	cout << " ------ Triangle Test ------" << endl << "Triangle Coords:" << endl << t.a << endl << t.b << endl << t.c << endl;

	light ll;
	pointLight llp;
	directionalLight lld;
	ambientLight lla;
	material mm;*/


	return 0;
}

