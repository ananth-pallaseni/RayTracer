
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

// Code from stackOVerflow for parsing input:
static inline string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

static inline string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

static inline string &trim(string &s) {
    return ltrim(rtrim(s));
}

vector<string> split(const string& str, const string& delimiter = " ") {
    vector <string> tokens;
    string::size_type lastPos = 0;
    string::size_type pos = str.find(delimiter, lastPos);
    while (string::npos != pos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.size();
        pos = str.find(delimiter, lastPos);
    }
    tokens.push_back(str.substr(lastPos, str.size() - lastPos));
    return tokens;
}
// End code from stackoverflow

 // Read Arguments and do something with them
void parseObj(const char* filename, material mat, Matrix4f objToWorld) {

  const int MAX_CHARS_PER_LINE = 512;
  const int MAX_TOKENS_PER_LINE = 20;
  const char* DELIMITER = " ";

  // create a file-reading object
    ifstream fin;
    fin.open(filename); // open a file
    string line;


    vector<Vector3f> vertices;
    Vector3f first;
    vertices.push_back(first);


    while(getline(fin, line)) {
      cout << line << endl;
    	vector<string> words = split(line);
    	if(words[0] == "v") {
    		stringstream f1(words[1]);
    		stringstream f2(words[2]);
    		stringstream f3(words[3]);
    		float p1;
    		f1 >> p1;
    		float p2;
    		f2 >> p2;
    		float p3;
    		f3 >> p3;
    		Vector3f vertex(p1, p2, p3);
        vertices.push_back(vertex);
        cout << "Vertex at : " << p1 << ", " << p2 << ", " << p3 << endl; 
    	}
    	if(words[0] == "f") {
    		stringstream f1(words[1]);
    		stringstream f2(words[2]);
    		stringstream f3(words[3]);
    		float p1;
    		f1 >> p1;
    		float p2;
    		f2 >> p2;
    		float p3;
    		f3 >> p3;
    		Vector3f v1 = vertices[p1];
    		Vector3f v2 = vertices[p2];
    		Vector3f v3 = vertices[p3];
    		triangle tri(v1(0), v1(1), v1(2), v2(0), v2(1), v2(2), v3(0), v3(1), v3(2), mat, objToWorld);
    		triangles.push_back(tri);
        cout << "Triangle" << endl;
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
  	  	else if(strcmp(token[0], "obj") == 0) {
  	  		cout << "token: " <<  token[1] << endl;
  			parseObj(token[1], mat, objToWorld);
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

	int size = 200;

	char* inFile = "input.txt";
  if(argc == 2) {
    inFile = argv[1];
  }
	clock_t startTime;
	double duration;
	startTime = clock();

	parseArgs(inFile);

	Sampler s(size, size, LL, LR, UL, UR);
	Canvas c(size, size);
	RayTracer rt(eye, spheres, triangles, pointLights, directionalLights, ambientLights);
	for(int i = 0 ; i < size; i++) {
		for(int j = 0 ; j < size; j++) {
      cout << i << ", " << j << endl;
			c.addPixel(rt.trace(s.getSample()));
			
		}
	}
	c.encode("image.png");

	duration = (clock() - startTime) / (double) CLOCKS_PER_SEC;
	cout << "DONE" << endl;
	cout << "TIME: " << duration << " seconds" << endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}

