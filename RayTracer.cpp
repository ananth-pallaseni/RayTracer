
#include <iostream>
#include "Eigen/Dense"
#include <fstream>
#include <string>

#include "Sampler.h"
#include "RayTracerObjects.h"

using namespace std;

// Read Arguments and do something with them
void parseArgs(char* filename) {
	fstream f(filename);
	string line;
	if(f.is_open()) {
		while(getline(f, line)) {
			cout << line << endl;
		}
		f.close();
	}
	else {
		cout << "Unable to open file " << filename << endl;
	}

}

int main(int argc, char* argv[])
{
	int xSize = 400;
	int ySize = 400;
	Vector2d q;
	Sampler s(400, 400, 0, 0, 400, 0, 400, 400, 0, 400);

	char* inFile = "input.txt";
	if(argc > 1) {
		inFile = argv[1];
	}
	cout << " ------ Input Parse Test ------ " << endl;
	parseArgs(inFile);
	cout << endl;

	// testing declarations	
	camera c(1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 8);
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
	material mm;


	return 0;
}

