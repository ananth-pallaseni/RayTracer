// RayTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Sampler.h"
#include "Eigen\Dense"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int xSize = 400;
	int ySize = 400;
	Vector2d q;
	Sampler s = Sampler(400, 400, 0, 0, 400, 0, 400, 400, 0, 400);
	for (int i = 0; i < 10; i++) {
		cout << s.getSample() << endl;
	}
	return 0;
}

