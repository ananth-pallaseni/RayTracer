
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

const char* const DELIMITER = " ";


// Read Arguments and do something with them
void parseArgs(char* filename) {
	fstream f(filename);
	string line;
	if(f.is_open()) {
		while(getline(f, line)) {
			cout << line << endl;
			const char* token[17] = {}; // initialize to 0
    
    		// parse the line
    		char* ll = line;
    		token[0] = strtok(ll, DELIMITER); // first token
    		if (token[0]) // zero if line is blank
    		{
    		  for (int n = 1; n < 17; n++)
    		  {
    		    token[n] = strtok(0, DELIMITER); // subsequent tokens
    		    if (!token[n]) break; // no more tokens
    		  }
    		}

    		for (int i = 0; i < n; i++) {// n = #of tokens
      			cout << "Token[" << i << "] = " << token[i] << endl;
      		}
    		cout << endl;


    		/*if(token[0]) {
    			char* start = token[0];
    			if (start == "cam") {

    			}
    		}*/
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

	string inFile = "input.txt";

	parseArgs(inFile);
	cout << endl;

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

