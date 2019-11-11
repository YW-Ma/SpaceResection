// SpaceIntersection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include"RESECTION.h"

int main()
{
	//for image No.9284
	string FileControl = "Data\\controlPts.txt";
	string FileImage = "Data\\9284pts.txt";
	string image = "Data\\IMG_9284.JPG";

	RESECTION resection(FileControl, FileImage, image);
	resection.Resection();
	//Orientation Elements
	resection.Xs = 185 * 10;
	resection.Ys = -9 * 10;
	resection.Zs = - 50 * 10;
	resection.Phi = 0.19;
	resection.Omega = -0.05;
	resection.Kappa = 0;
	resection.x0 = 0;
	resection.y0 = 0;


	//for image No.9282
	//string FileControl = "Data\\controlPts.txt";
	//string FileImage = "Data\\9282pts.txt";
	//string image = "Data\\IMG_9282.JPG";

	//RESECTION resection(FileControl, FileImage, image);
	//resection.Resection();
	//Orientation Elements
	//for image No.9282
	//resection.Xs = 460 * 10;
	//resection.Ys = -28 * 10;
	//resection.Zs = -80 * 10;
	//resection.Phi = -0.45;
	//resection.Omega = -0.1;
	//resection.Kappa = 0;
	//resection.x0 = 0;
	//resection.y0 = 0;


	return 0;
}
