
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
#include "AABBTree.h"

using namespace std;


vector<sphere> spheres;
vector<triangle> triangles;
vector<pointLight> pointLights;
vector<directionalLight> directionalLights;
vector<ambientLight> ambientLights;
vector<boundingBox> boxes;

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



  /*  while(getline(fin, line, ' ')) {
    	cout << line << endl;
    	if(line == "v") {
    		getline(fin, line, ' ');
    		float a1 = atof(line.cstring());
    		getline(fin, line, ' ');
    		float a2 = atof(line.cstring());
    		getline(fin, line, ' ');
    		float a3 = atof(line.cstring());
    		Vector3f vert(a1, a2, a3);
    		vertices.push_back(vert);
    	}
    	if(line == "f") {
    		getline(fin, line, ' ');
    		float a1 = atof(line.cstring());
    		getline(fin, line, ' ');
    		float a2 = atof(line.cstring());
    		getline(fin, line, ' ');
    		float a3 = atof(line.cstring());
    		Vector3f v1 = vertices[a1];
    		Vector3f v2 = vertices[a2];
    		Vector3f v3 = vertices[a3];
    		triangle tri(v1(0), v1(1), v1(2), v2(0), v2(1), v2(2), v3(0), v3(1), v3(2), mat, objToWorld);
    	}
    }*/

 
    /*
    // read each line of the file
    while (!fin.eof())
    {
    	cout << "2" << endl;
        // read an entire line into memory
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        
        cout << "BUF: " << buf << endl;
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
*/

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



/*boundingBox distill(vector<boundingBox*> boxIn) {
  vector<boundingBox*> distilledBoxes;
  int length = boxIn.size();

  cout << endl << "NEW ROUND, size: " << length << endl;
  if(length == 1) {
    cout << "LENGTH IS 1, RETURNING" << endl;
    cout << "Size: " << boxIn[0]->volume << "   minX: " << boxIn[0]->minX << endl;
    return *boxIn[0];
  }

  bool active[length];
  for(int i = 0; i < length; i++) {
    active[i] = true;
  }

  for(int i = 0; i < length; i++) {
    cout << "looking at new i = " << i << endl;
    if(active[i]) {
      cout << "NEW ITEM FOUND: " << boxIn[i]->volume <<  endl;
      int index = -1;
      boundingBox best;
      float minVol = std::numeric_limits<float>::max();
      for(int j = i + 1; j < length; j++) {
        cout << "looking at new j = " << j << endl;
        if(active[j]) {
          boundingBox combined(boxIn[i], boxIn[j]);
          cout << "NEW J FOUND: " << boxIn[j]->minX << ", " << boxIn[j]->maxX << "   v: " << combined.volume << "  comb minX: " << combined.minX <<  endl;
          if(combined.volume < minVol) {
            cout << "choosing this j" << endl;
            best = combined;
            index = j;
            minVol = combined.volume;
          }
        }
      }
      if(index >= 0) {
        cout << "setting j value to false" << endl;
        active[index] = false;
      }
      else {
        cout << "NO BEST FOUND" << endl;
        best = *boxIn[i];
      }
      
      distilledBoxes.push_back(&best);
    }

  }
  return distill(distilledBoxes);
}*/

boundingBox distill(vector<boundingBox> boxIn) {
  int length = boxIn.size();
  if(length == 1) {
    cout << "SIZE 1: RETURNING" << endl;
    cout << "root: " << boxIn[0].minX << endl;
    cout << "0, LEFT " <<  boxIn[0].left -> minX << endl;
    cout << "0, RIGHT " <<  boxIn[0].right -> minX << endl;
    return boxIn[0];
  }

  if(length == 3) {
    cout << ((sphere*) ( ( boxIn[0].left )->obj ) )->center(0) << endl;
    cout << "0, RIGHT " <<  boxIn[0].right -> minX << endl;
    cout << "1, LEFT " <<  boxIn[1].left -> minX << endl;
    cout << "1, RIGHT " <<  boxIn[1].right -> minX << endl;
  }

  if(length == 2) {
    cout << "root : " << boxIn[0].minX << endl;
    cout << "root -> left : " <<  boxIn[0].left ->  minX << endl;
    cout << "root -> left -> left: " <<  boxIn[0].left -> left -> minX << endl;
    cout << "root -> left -> right: " <<  boxIn[0].left -> right -> minX << endl;
    cout << "root -> right : " <<  boxIn[0].right ->  minX << endl;
    cout << "root -> right -> left: " <<  boxIn[0].right -> left -> minX << endl;
    cout << "root -> right -> right: " <<  boxIn[0].right -> right -> minX << endl;
  }

  vector<boundingBox> distilledBox;

  bool active[length];
  for(int i = 0; i < length; i++) {
    active[i] = true;
  }


  for(int i = 0; i < length; i++) {
    cout << "new i: " << i << endl;
    if(active[i]) {
      cout << "i was active" << endl;
      if(i == length-1) {
        cout << "i was last" << endl;
        cout << "i min: " << boxIn[i].minX << endl;
        distilledBox.push_back(boxIn[i]);
      }
      else {
        cout << "i min: " << boxIn[i].minX << " i+1 min: " << boxIn[i+1].minX << endl;
        boundingBox b(&boxIn[i], &boxIn[i+1]);
        cout << "comb min: " << b.minX << endl;
        distilledBox.push_back(b);
      }
      active[i] = false;
      active[i+1] = false;
    }

  }


  return distill(distilledBox);
}


bool rayTraverse(ray* r, boundingBox* b, hitResult* result) {
  if(b->hit(r, result)) {
    if(b->leaf) {
      return true;
    }
    hitResult r1;
    hitResult r2;
    bool check1 = rayTraverse(r, b->left, &r1);
    bool check2 = rayTraverse(r, b->right, &r2);
    if(check1 && check2) {
      if(r1.t < r2.t) {
        result = &r1;
      }
      else {
        result = &r2;
      }
      return true;
    }
    else if(check1) {
      result = &r1;
      return true;
    }
    else if(check2) {
      result = &r2;
      return true;
    }
  }
  return false;
}



Vector3f sphereNormal1(Vector3f pointOnShape, sphere shape) {
  Vector3f normal = pointOnShape - shape.center;
  Matrix4f m = shape.objToWorld;
  Matrix3f normTransform;
  normTransform << m(1, 1) * m(2, 2) - m(1, 2) * m(2 , 1), m(1, 2) * m(3,1) - m(1, 0) * m(2 ,2), m(1,0)*m(2,1) - m(1,1)*m(2,0),
           m(0,2)*m(2,1) - m(0,1)*m(2,2), m(0,0)*m(2,2)-m(0,2)*m(2,0), m(0,1)*m(2,0)-m(0,0)*m(2,1),
           m(0,1)*m(1,2)-m(0,1)*m(1,1), m(0,2)*m(1,0)-m(0,0)*m(1,2), m(0,0)*m(1,1)-m(0,1)*m(1,0);
  normal = normTransform * normal;
  return normal;
}

int main(int argc, char* argv[])
{
material mm;

  /*material mm;
  Matrix4f I;
    I << 1, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1;

  sphere s1(0, 0, 0, 20, mm, I, I);
  sphere s2(50, 50, 50, 20, mm, I, I);
  sphere s3(100, 100, 100, 20, mm, I, I);
  sphere s4(150, 150, 150, 20, mm, I, I);
  sphere s5(200, 200, 200, 20, mm, I, I);
  boundingBox b1(&s1);
  boundingBox b2(&s2);
  boundingBox b3(&s3);
  boundingBox b4(&s4);
  boundingBox b5(&s5);
  boundingBox b11(&b1, &b2);
  boundingBox b22(&b3, &b4);
  boundingBox b33(&b11, &b22);
  boundingBox b44(&b33, &b5);

  cout << "B1 MINX: " << b44.left->left->left->minX << endl;

  Vector3f e1(-30, -20, 0);
  Vector3f d1(1, 0, 0);
  ray r1(e1, d1);
  hitResult result;
  if(rayTraverse(&r1, &b44, &result)) {
    cout << "RAY HIT" << endl;
    cout << result.point << endl << endl;
  }
  else {
    cout << "NO HIT" << endl;
  }*/



  /*material mm;
  Matrix4f I;
    I << 1, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1;
  
  for(int i = 0; i < 5; i++) {
    sphere sph(i * 50, i * 50, i*50, 20, mm, I, I);
    spheres.push_back(sph);
  }*/

  /*for(int i = 0; i < 5; i++) {
    boundingBox b(&spheres[i]);
    cout << "BOX " << i << endl;
    cout << "minx: " << b.minX << " maxx: " << b.maxX << endl;
    cout << "minY: " << b.minY << " maxy: " << b.maxY << endl;
    cout << "minZ: " << b.minZ << " maxZ: " << b.maxZ << endl;
    boxes.push_back(b);
  }

  cout << endl;


  boundingBox tree = distill(boxes);
  cout << "AFTER FUNC, root: minx " << tree.minX << endl;
  cout << "AFTER FUNC, root: maxx " << tree.maxX << endl;
  cout << "AFTER FUNC, root: miny " << tree.minY << endl;
  cout << "AFTER FUNC, root: maxy " << tree.maxY << endl;
  cout << "AFTER FUNC, root: minz " << tree.minZ << endl;
  cout << "AFTER FUNC, root: maxz " << tree.maxZ << endl;

  cout << "AFTER FUNC, root -> left: minx " << tree.left -> minX << endl;
  cout << "AFTER FUNC, root -> left: maxx " << tree.left -> maxX << endl;
  cout << "AFTER FUNC, root -> left: miny " << tree.left -> minY << endl;
  cout << "AFTER FUNC, root -> left: maxy " << tree.left -> maxY << endl;
  cout << "AFTER FUNC, root -> left: minz " << tree.left -> minZ << endl;
  cout << "AFTER FUNC, root -> left: maxz " << tree.left -> maxZ << endl;

  cout << "AFTER FUNC, root -> left -> left: minx " << tree.left -> left -> minX << endl;
  cout << "AFTER FUNC, root -> left -> left: maxx " << tree.left -> left -> maxX << endl;
  cout << "AFTER FUNC, root -> left -> left: miny " << tree.left -> left -> minY << endl;
  cout << "AFTER FUNC, root -> left -> left: maxy " << tree.left -> left -> maxY << endl;
  cout << "AFTER FUNC, root -> left -> left: minz " << tree.left -> left -> minZ << endl;
  cout << "AFTER FUNC, root -> left -> left: maxz " << tree.left -> left -> maxZ << endl;

  cout << "AFTER FUNC, root -> left -> right: minx " << tree.left -> right -> minX << endl;
  cout << "AFTER FUNC, root -> left -> right: maxx " << tree.left -> right -> maxX << endl;
  cout << "AFTER FUNC, root -> left -> right: miny " << tree.left -> right -> minY << endl;
  cout << "AFTER FUNC, root -> left -> right: maxy " << tree.left -> right -> maxY << endl;
  cout << "AFTER FUNC, root -> left -> right: minz " << tree.left -> right -> minZ << endl;
  cout << "AFTER FUNC, root -> left -> right: maxz " << tree.left -> right -> maxZ << endl;

  cout << "AFTER FUNC, root -> left -> left -> left: minx " << tree.left -> left -> left -> minX << endl;
  cout << "AFTER FUNC, root -> left -> left -> left: maxx " << tree.left -> left -> left -> maxX << endl;
  cout << "AFTER FUNC, root -> left -> left -> left: miny " << tree.left -> left -> left -> minY << endl;
  cout << "AFTER FUNC, root -> left -> left -> left: maxy " << tree.left -> left -> left -> maxY << endl;
  cout << "AFTER FUNC, root -> left -> left -> left: minz " << tree.left -> left -> left -> minZ << endl;
  cout << "AFTER FUNC, root -> left -> left -> left: maxz " << tree.left -> left -> left -> maxZ << endl;

  
  Vector3f e1(-30, -20, 0);
  Vector3f d1(1, 0, 0);
  ray r1(e1, d1);
  hitResult result;
  if(rayTraverse(&r1, &tree, &result)) {
    cout << "RAY HIT" << endl;
    cout << result.point << endl << endl;
  }
  else {
    cout << "NO HIT" << endl;
  }*/

   /* cout << "r hits sph0: " << r1.intersect(spheres[0], &result) << endl;

  //traverse(&tree);

  cout << "HITS OUTER: " << tree.hit(&r1, &result) << endl;
  cout << "HITS OUTER->LEFT: " << (*(tree.left)).hit(&r1, &result) << endl;
  cout << "OUTER-> LEFT: " << endl << (*(tree.left)) . minX << endl;
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// General Purpose - renders input file

	int size = 1000;

	char* inFile = "input.txt";
	clock_t startTime;
	double duration;
	startTime = clock();

	parseArgs(inFile);
  cout << "PARSED INPUTS, Adding Boxes" << endl;

  for(int i = 0; i < spheres.size() ; i++) {
    boundingBox b(&spheres[i]);
    boxes.push_back(b);
  }

  for(int i = 0; i < triangles.size() ; i++) {
    boundingBox b(&triangles[i]);
    boxes.push_back(b);
  }

  cout << "ADDED BOXES, BEGINNING TRACE" << endl;

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
	cout << "TIME: " << duration << " seconds" << endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}

