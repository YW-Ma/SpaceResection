// PhotogrammetrySpaceResection.cpp: 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LOOP 100
#include "stdafx.h"
#include <vector>
#include <string>
#include "Matrix.h"
#include <iostream>
#include <cmath>
//coordinate in image
struct imgPt
{
	double x;
	double y;
};
//coordinate in ground
struct gPt
{
	double X;
	double Y;
	double Z;
};

int main()
{
	//1. Read data files
	//1.1 Read controlPoints.txt
	std::vector<imgPt> imgPts;
	std::vector<gPt> gPts;
	std::string control_Pts_File_Name = "controlPoints.txt";
	FILE* fp_CP = fopen(control_Pts_File_Name.data(), "r");
	if (fp_CP==nullptr)
	{
		printf("ERROR: Cannot Open controlPoints.txt\n");
		return 1;
	}
	else
	{
		while (!feof(fp_CP))
		{
			double x, y, X, Y, Z;
			imgPt t_imgPt;
			gPt t_gPt;
			fscanf(fp_CP, "%lf,%lf,%lf,%lf,%lf,", &x, &y, &X, &Y, &Z);
			printf("read Points:%lf,%lf,%lf,%lf,%lf\n", x, y, X, Y, Z);
			t_imgPt.x = x;
			t_imgPt.y = y;
			t_gPt.X = X;
			t_gPt.Y = Y;
			t_gPt.Z = Z;
			imgPts.push_back(t_imgPt);
			gPts.push_back(t_gPt);
		}
	}
	//1.2 Read interiorOrientation.txt and scale.txt
	double fk = 0;
	double x0 = 0;
	double y0 = 0;
	double height = 0;
	double scale = 0;
	std::string inter_File_Name = "interiorOrientation.txt";
	std::string scale_File_Name = "scale.txt";
	FILE* fp_IO = fopen(inter_File_Name.data(), "r");
	FILE* fp_SC = fopen(scale_File_Name.data(), "r");
	if (fp_IO == nullptr)
	{
		printf("ERROR: Cannot Open interiorOrientation.txt\n");
		return 2;
	}
	else
	{
		fscanf(fp_IO, "%lf\n", &fk);
		fscanf(fp_IO, "%lf\n", &x0);
		fscanf(fp_IO, "%lf\n", &y0);
	}
	if (fp_SC == nullptr)
	{
		printf("ERROR: Cannot Open scale.txt\n");
		return 3;
	}
	else
	{
		fscanf(fp_SC, "%lf\n", &scale);
	}
	height = scale * fk;

	//1.3 to correct imgPts
	for (int i = 0; i < imgPts.size; i++)
	{
		imgPts.at(i).x = imgPts.at(i).x - x0;
		imgPts.at(i).y = imgPts.at(i).y - y0;
	}

	//2 initialization elements of exteror orientation
	double Zs = height;
	double t1 = 0;
	double t2 = 0;
	for (int i = 0; i < gPts.size; i++)
	{
		t1 += gPts.at(i).X;
		t2 += gPts.at(i).Y;
	}
	double Xs = t1 / gPts.size;
	double Ys = t2 / gPts.size;
	double Phi = 0;
	double Omega = 0;
	double Kappa = 0;

	//3 calculate rotation matrix R in Chinese Standard
	Matrix X0(6, 1);
	X0.data[0] = Xs;
	X0.data[1] = Ys;
	X0.data[2] = Zs;
	X0.data[3] = Phi;
	X0.data[4] = Omega;
	X0.data[5] = Kappa;

	int loop = 1;
	bool convergence = false;

	while (loop < MAX_LOOP)
	{
		double Phi = X0.data[3];
		double Omega = X0.data[4];
		double Kappa = X0.data[5];
		Matrix R(3, 3);//default init = 0
		R.data[0*R.col+0] = cos(Phi)*cos(Kappa) - sin(Phi)*sin(Omega)*sin(Kappa);
		R.data[0*R.col+1] = -cos(Phi)*sin(Kappa) - sin(Phi)*sin(Omega)*cos(Kappa);
		R.data[0*R.col+2] = -sin(Phi)*cos(Omega);
		R.data[1*R.col+0] = cos(Omega)*sin(Kappa);
		R.data[1*R.col+1] = cos(Omega)*cos(Kappa);
		R.data[1*R.col+2] = -sin(Omega);
		R.data[2*R.col+0] = sin(Phi)*cos(Kappa) + cos(Phi)*sin(Omega)*sin(Kappa);
		R.data[2*R.col+1] = -sin(Phi)*sin(Kappa) + cos(Phi)*sin(Omega)*cos(Kappa);
		R.data[2*R.col+2] = cos(Phi)*cos(Omega);

	}
    return 0;
}

