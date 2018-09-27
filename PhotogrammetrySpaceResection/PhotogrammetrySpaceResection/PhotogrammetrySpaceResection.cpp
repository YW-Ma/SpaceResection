// PhotogrammetrySpaceResection.cpp: 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <vector>
#include <string>
#include "Matrix.h"
#include <iostream>
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


	//test
	Matrix test(5, 5);
	Matrix invOfA(5, 5);
	srand((unsigned)time(0));
	int N = test.row;
	for (int i = 0; i<N; i++)
	{
		for (int j = 0; j<N; j++)
		{
			test.data[i*N + j] = rand() % 100 * 0.1;
		}
	}
	test.LUP_inverse(invOfA);

	std::cout << "A:" << std::endl;
	for (int i = 0; i<N; i++)
	{
		for (int j = 0; j<N; j++)
		{
			std::cout << test.data[i*N + j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "A_inversed:" << std::endl;
	for (int i = 0; i<N; i++)
	{
		for (int j = 0; j<N; j++)
		{
			std::cout << invOfA.data[i*N + j] << " ";
		}
		std::cout << std::endl;
	}
    return 0;
}

