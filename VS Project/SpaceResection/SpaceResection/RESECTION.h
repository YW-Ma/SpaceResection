#pragma once
#include"pch.h"
#define DEG2RAD 0.01745329
using namespace std;
using namespace cv;

/****************************************************************/
//关于控制场 和 像点的坐标
//控制点坐标：面向控制场，X向前，Y向右，Z向上。是左手坐标系；
//像点坐标：面向图像，x向右，y向下，z向前。是右手坐标系；
/****************************************************************/
/****************************************************************/
//
//空间后方交会
//
//误差方程式： V = At + CX2 + DX - L
//含义：
//	A/C/D：系数阵
//	t：外方位元素的改正数
//	X2：内方位元素的改正数
//	X：畸变系数的改正数
//	L：像点观测值 - 估计值（估计值是求出来的）
//
/****************************************************************/

class RESECTION
{
public:
	bool disp = false;//是否输出调试信息
	string control_file_path;
	string image_file_path;
	string image;
	//可以根据实际需求在主函数中修改RESECTION对象的参数初值
	double Xs;	//外方位线元素Xs初值
	double Ys;	//外方位线元素Ys初值
	double Zs;	//外方位线元素Zs初值
	double Phi;	//外方位角元素Phi初值
	double Omega;//外方位角元素Omega初值
	double Kappa;//外方位角元素Omega初值
	double x0;	//像主点的x0初值
	double y0;	//像主点的y0初值
	double f;	//主距初值
	double k1;	//畸变参数k1初值
	double k2;	//畸变参数k2初值
	double p1;	//畸变参数p1初值
	double p2;	//畸变参数p1初值

private:
	Mat img;			//存储图像
	Mat controlPts;			  //[ID,X,Y,Z]
	vector<ImagePt> imagePts; //[ID,x,y,X,Y,Z]
	int imagePtsNum;	//像点个数
	int equationNum;	//方程的个数。每个像点的x和y分别有一个方程，共有两个。
	int elementNum;		//未知数个数，本次实验中为13；
	Mat R;				//旋转矩阵
						// [V = At + CX2 + DX - L] ==> [V=AX-L]
	Mat V;				//一个像点对应两个方程
	Mat A;				//A/C/D系数矩阵合一个矩阵
	Mat X;				//所有的未知数合一个矩阵
	Mat L;
	int itrNum;			//迭代次数
public:
	RESECTION(string control_file_path, string image_file_path, string image);
	~RESECTION();
	void Resection();
private:
	int readControlPts(string control_file_path);//功能：读取控制点坐标文件
	int readImagePts(string image_file_path);//功能：读取像点坐标文件
	void updateRAL();//重新计算矩阵 R\A\L
	void updateX();
	void writeResults();//输出结果
};
