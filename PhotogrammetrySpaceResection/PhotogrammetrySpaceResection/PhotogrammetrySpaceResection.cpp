// PhotogrammetrySpaceResection.cpp: 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS
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
	if (fp_CP == nullptr)
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
			if (feof(fp_CP))//to prevent programme from reading last line twice.
				break;
			t_imgPt.x = x/1000.0f;//use meter instead of millimeter as unit
			t_imgPt.y = y/1000.0f;
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
	// use meter as unit
	fk = fk / 1000.0f;
	x0 = x0 / 1000.0f;
	y0 = y0 / 1000.0f;
	height = scale * fk;

	//1.3 to correct imgPts
	for (int i = 0; i < imgPts.size(); i++)
	{
		imgPts.at(i).x = imgPts.at(i).x - x0;
		imgPts.at(i).y = imgPts.at(i).y - y0;
	}

	//2 initialization elements of exteror orientation
	double Zs = height;
	double t1 = 0;
	double t2 = 0;
	for (int i = 0; i < gPts.size(); i++)
	{
		t1 += gPts.at(i).X;
		t2 += gPts.at(i).Y;
	}
	double Xs = t1 / gPts.size();
	double Ys = t2 / gPts.size();
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
	int max_loop = 1000;
	int nPts = gPts.size();
	Matrix Q(6, 6);//Q = inv(A'A) in 5.2 section
	Matrix A(2 * nPts, 6);//store coefficient
	Matrix L(2 * nPts, 1);//store result 
	Matrix X(6, 1);//store correction value of each loop; used in 5.2
	Matrix R(3, 3);//default init = 0
	while (loop < max_loop)
	{
		double Xs = X0.data[0];
		double Ys = X0.data[1];
		double Zs = X0.data[2];
		double Phi = X0.data[3];
		double Omega = X0.data[4];
		double Kappa = X0.data[5];

		R.data[0 * R.col + 0] = cos(Phi)*cos(Kappa) - sin(Phi)*sin(Omega)*sin(Kappa);
		R.data[0 * R.col + 1] = -cos(Phi)*sin(Kappa) - sin(Phi)*sin(Omega)*cos(Kappa);
		R.data[0 * R.col + 2] = -sin(Phi)*cos(Omega);
		R.data[1 * R.col + 0] = cos(Omega)*sin(Kappa);
		R.data[1 * R.col + 1] = cos(Omega)*cos(Kappa);
		R.data[1 * R.col + 2] = -sin(Omega);
		R.data[2 * R.col + 0] = sin(Phi)*cos(Kappa) + cos(Phi)*sin(Omega)*sin(Kappa);
		R.data[2 * R.col + 1] = -sin(Phi)*sin(Kappa) + cos(Phi)*sin(Omega)*cos(Kappa);
		R.data[2 * R.col + 2] = cos(Phi)*cos(Omega);
		//4. Calculate Lx Ly (error values)
		double& a1 = R.data[0 * R.col + 0];
		double& a2 = R.data[0 * R.col + 1];
		double& a3 = R.data[0 * R.col + 2];

		double& b1 = R.data[1 * R.col + 0];
		double& b2 = R.data[1 * R.col + 1];
		double& b3 = R.data[1 * R.col + 2];

		double& c1 = R.data[2 * R.col + 0];
		double& c2 = R.data[2 * R.col + 1];
		double& c3 = R.data[2 * R.col + 2];

		for (int i = 0; i < nPts; i++)
		{
			//4.1 calculate A matrix
			double X = gPts.at(i).X;
			double Y = gPts.at(i).Y;
			double Z = gPts.at(i).Z;
			double x = imgPts.at(i).x;
			double y = imgPts.at(i).y;

			double X_ = a1 * (X - Xs) + b1 * (Y - Ys) + c1 * (Z - Zs);
			double Y_ = a2 * (X - Xs) + b2 * (Y - Ys) + c2 * (Z - Zs);
			double Z_ = a3 * (X - Xs) + b3 * (Y - Ys) + c3 * (Z - Zs);

			A.data[(0 + 2 * i)*A.col + 0] = 1 / Z_ * (a1*fk + a3 * (x - x0));
			A.data[(0 + 2 * i)*A.col+1] = 1 / Z_ * (b1*fk + b3*(x - x0));
			A.data[(0 + 2 * i)*A.col+2] = 1 / Z_ * (c1*fk + c3*(x - x0));
			A.data[(0 + 2 * i)*A.col+3] = (y - y0)*sin(Omega) - ((x - x0) / fk * ((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) + fk * cos(Kappa))*cos(Omega);
			A.data[(0 + 2 * i)*A.col+4] = -fk * sin(Kappa) - (x - x0) / fk * ((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
			A.data[(0 + 2 * i)*A.col+5] = y - y0;

			A.data[(1 + 2 * i)*A.col + 0] = 1 / Z_ * (a2*fk + a3 * (y - y0));
			A.data[(1 + 2 * i)*A.col+1] = 1 / Z_ * (b2*fk + b3*(y - y0));
			A.data[(1 + 2 * i)*A.col+2] = 1 / Z_ * (c2*fk + c3*(y - y0));
			A.data[(1 + 2 * i)*A.col+3] = (x - x0)*sin(Omega) - ((y - y0) / fk * ((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) + fk * sin(Kappa))*cos(Omega);
			A.data[(1 + 2 * i)*A.col+4] = -fk * cos(Kappa) - (y - y0) / fk * ((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
			A.data[(1 + 2 * i)*A.col+5] = -(x - x0);
			//4.2
			//L0=[xl,yl]'=[-f*X_/Z_,-f*Y_/Z_]'
			//L(2*i-1:2*i,:)=imgPts(i,:)'-L0;
			L.data[2 * i] = imgPts.at(i).x - (-fk * X_ / Z_);
			L.data[2 * i + 1] = imgPts.at(i).y - (-fk * Y_ / Z_);
		}
		//5. V=AX-L; X=inv(A'A)A'L;
		//5.1 create A' and A
		Matrix At(2 * nPts,6);
		A.clone(At);
		At.trans();
		//5.2 Q = inv(A'A)
		Matrix Q_temp(6, 6);
		At.multiply(A, Q_temp);//Q_temp = A'A
		Q_temp.LUP_inverse(Q); //Q=inv(Q_temp)
		Matrix temp_QAt(6, 8);
		Q.multiply(At, temp_QAt);
		X.set_zero();
		temp_QAt.multiply(L, X);
		X0.add(X);
		//6. Convergence Judgement
		if (abs(L.avg()) < 0.000001)
		{
			convergence = true;
			break;
		}
		L.set_zero();
		A.set_zero();
		loop++;
	}

	//7.show result
	if (convergence==true)
	{
		//7.1 accuracy assessment
		Matrix V(2 * nPts, 1);
		A.multiply(X, V);
		V.minus(L);
		//7.2 display result and acuracy
		X0.show();
		R.show();
		printf("loop %d\n", loop);
	}
	else
	{
		printf("Do not achieve Convergence after %d loops\n", loop-1);
	}
	return 0;
}

