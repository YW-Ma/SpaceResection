#include "pch.h"
#include "RESECTION.h"




//构造函数：读入所有所需的数据
RESECTION::RESECTION(string control_file_path0, string image_file_path0, string image0)
{
	cout << "******** Space Resection ********" << endl;
	cout << "Step 1: Read object space coordinates and pixel coordinates of the control point." << endl;
	readControlPts(control_file_path0);
	readImagePts(image_file_path0);
	control_file_path = control_file_path0;
	image_file_path = image_file_path0;
	image = image0;
	img = imread(image);
	namedWindow("iteration", cv::WINDOW_NORMAL);
	imshow("iteration", img);

	//给未知数赋初值(畸变就写到k2就好了)
	cout << "Step 2: Assign initial values to 13 unknowns since we use Gauss-Newton Method." << endl;

	//f = 6157.84; //32mm为主距参数,5.1966e-3mm为估计像素大小，6157.84为以像素为坐标的主距初值
	f = 3940;
	k1 = 0;
	k2 = 0;
	p1 = 0;
	p2 = 0;

	//给迭代部分要用到的矩阵分配空间
	elementNum = 13;
	V = Mat(equationNum, 1, CV_32FC1);
	A = Mat(equationNum, elementNum, CV_32FC1);
	X = Mat(equationNum, 1, CV_32FC1);
	L = Mat(equationNum, 1, CV_32FC1);
	R = Mat(3, 3, CV_32FC1);

	itrNum = 0;
}
//析构函数：释放分配的内存
RESECTION::~RESECTION()
{
}
void RESECTION::Resection()
{
	cout << "Step 3: Initialize the error equation and then start using the Gauss-Newton method to calculate the value of the unknowns iteratively" << endl;
	//初次UpdateRAL
	updateRAL();
	//更新X矩阵
	updateX();
	itrNum++;
	//然后是while(还没收敛) 则UpdateRAL
	bool notConverge = (abs(X.at<float>(3, 0)) > 1e-5 || abs(X.at<float>(4, 0)) > 1e-5 || abs(X.at<float>(5, 0)) > 1e-5);
	while (notConverge)
	{
		updateRAL();
		updateX();
		notConverge = (abs(X.at<float>(3, 0)) > 1e-5 || abs(X.at<float>(4, 0)) > 1e-5 || abs(X.at<float>(5, 0)) > 1e-5);
		itrNum++;
	}
	//最后输出结果writeResults
	cout << "Step 4: Write results" << endl;
	cout << "******** The End ********" << endl;
	writeResults();
}
int RESECTION::readControlPts(string control_file_path)//功能：读取控制点坐标文件
{
	ifstream ifs;
	ifs.open(control_file_path);
	if (!ifs)
	{
		printf("Fail to read control points\n");
		return 0;
	}
	int pts_num = 0;
	//先读取总点数
	ifs >> pts_num;
	printf("  Num of Control Pts：%d\n", pts_num);
	controlPts = Mat(pts_num, 4, CV_32FC1);//32位浮点，单通道。实际上就是浮点矩阵。
	float ID, X, Y, Z;
	int cnt = 0;
	while (ifs >> ID >> X >> Y >> Z)
	{
		if (disp)//调试信息
		{
			cout << ID << " " << X << " " << Y << " " << Z << endl;
		}
		//存在一行中
		controlPts.at<float>(cnt, 0) = ID;
		controlPts.at<float>(cnt, 1) = X;
		controlPts.at<float>(cnt, 2) = Y;
		controlPts.at<float>(cnt, 3) = Z;
		cnt++;
	}
	ifs.close();
	return 1;
}

int RESECTION::readImagePts(string image_file_path)//功能：读取像点坐标文件
{
	ifstream ifs;
	ifs.open(image_file_path);
	if (!ifs)
	{
		printf("Fail to read image points\n");
		return 0;
	}
	int pts_num = 0;
	//像点数可变，利用vector临时获取
	float ID, x, y;
	cout << "----------\nTip: This program is suitable for data obtained from close-range photogrammetry.\n If you need to use data acquired by aerial photogrammetry, try swapping the XYZ coordinates\n so that they match the close-up photogrammetry.\n----------" << endl;
	while (ifs >> ID >> x >> y)
	{
		//在本次实验中存在未知点，在空间后方交会中用不上，需要跳过。
		//这些未知点的序号都是 两位数，而控制点的序号都是 三位数，所以才跳过所有的两位数。
		//注意：如果是没有这些两位数序号的数据，请务必注释掉这段if语句。
		//There are unknown points in this experiment, which are useless in the resection of space, and therefore need to be skipped.
		//The numbers of these unknown points are two digits, and the numbers of the control points are all three digits, so point numbers which are lower than 100 are skipped.
		//Note: If it is normal data without useless two digits points, be sure to comment out the if statement.
		if (ID < 100)
		{
			continue;//跳过未知点
		}
		ImagePt temp;
		temp.ID = ID;
		temp.x = x;
		temp.y = y;
		//在控制点数据中知道这个ID对应的[X,Y,Z]
		for (int i = 0; i < controlPts.rows; i++)
		{
			if (controlPts.at<float>(i, 0) == ID)
			{
				//在这里要变换一下坐标系----如果控制点是航空摄影测量下的，则直接按顺序读取123位即可
				//X = Yo，Y = Zo，Z = -Xo
				temp.X = controlPts.at<float>(i, 2);
				temp.Y = controlPts.at<float>(i, 3);
				temp.Z = -controlPts.at<float>(i, 1);
				break;//就可以退出啦
			}
		}
		if (disp)//调试信息
		{
			cout << ID << " " << x << " " << y << " " << temp.X << " " << temp.Y << " " << temp.Z << endl;
		}
		imagePts.push_back(temp);
		pts_num++;
	}
	imagePtsNum = pts_num;
	equationNum = 2 * pts_num;
	ifs.close();
	return 1;
}

void RESECTION::updateRAL()//重新计算矩阵 R\A\L
{
	//先计算R，R=R1 x R2 x R3;
	R.at<float>(0, 0) = cos(Phi)*cos(Kappa) - sin(Phi)*sin(Omega)*sin(Kappa);
	R.at<float>(0, 1) = cos(Omega)*sin(Kappa);
	R.at<float>(0, 2) = sin(Phi)*cos(Kappa) + cos(Phi)*sin(Omega)*sin(Kappa);
	R.at<float>(1, 0) = -cos(Phi)*sin(Kappa) - sin(Phi)*sin(Omega)*cos(Kappa);
	R.at<float>(1, 1) = cos(Omega)*cos(Kappa);
	R.at<float>(1, 2) = -sin(Phi)*sin(Kappa) + cos(Phi)*sin(Omega)*cos(Kappa);
	R.at<float>(2, 0) = -sin(Phi)*cos(Omega);
	R.at<float>(2, 1) = -sin(Omega);
	R.at<float>(2, 2) = cos(Phi)*cos(Omega);
	//再逐点计算出误差方程
	Mat imgt = img.clone();
	for (int i = 0; i < imagePtsNum; i++)
	{
		// 矩阵赋值[下标i代表第i个点][一个点会有两个方程]
		//1.提取值
		double x = imagePts.at(i).x;//观测值
		double y = imagePts.at(i).y;
		double Xi = imagePts.at(i).X;
		double Yi = imagePts.at(i).Y;
		double Zi = imagePts.at(i).Z;
		//2.列共线方程
		double XR = R.at<float>(0, 0)*(Xi - Xs) + R.at<float>(0, 1)*(Yi - Ys) + R.at<float>(0, 2)*(Zi - Zs);
		double YR = R.at<float>(1, 0)*(Xi - Xs) + R.at<float>(1, 1)*(Yi - Ys) + R.at<float>(1, 2)*(Zi - Zs);
		double ZR = R.at<float>(2, 0)*(Xi - Xs) + R.at<float>(2, 1)*(Yi - Ys) + R.at<float>(2, 2)*(Zi - Zs);
		double xi = -f * XR / ZR + x0;//估计值
		double yi = -f * YR / ZR + y0;
		//改正像点估计值坐标
		double r = sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
		//改正值的计算方法是：
			// x = (xi-x0)
			//dx = (x)[k1*r^2 + k2*r^4] + [2*p1*x*y + p2*(r^2+2*x^2)]
			//dy = (y)[k1*r^2 + k2*r^4] + [p1*(r^2+2*y^2) + 2*p2*x*y]
		double dx = (x - x0)*(k1 * powf(r, 2) + k2 * powf(r, 4)) + 2 * p1*(x - x0)*(y - y0) + p2 * (powf(r, 2) + 2 * powf((x - x0), 2));
		double dy = (y - y0)*(k1 * powf(r, 2) + k2 * powf(r, 4)) + p1 * (powf(r, 2) + 2 * powf((y - y0), 2)) + 2 * p2*(x - x0)*(y - y0);
		xi = (xi - dx);//畸变纠正后估计值
		yi = (yi - dy);
		//3.1.第一个方程（对x) 
		L.at<float>(2 * i, 0) = x - xi;//观测值-畸变纠正后估计值
		//(x-x0)[观测值] = (xi-dx)[估计值] --> (xi - [观测值]*畸变系数)
		//更新A矩阵
		A.at<float>(2 * i, 0) = (R.at<float>(0, 0)*f + R.at<float>(2, 0)*(x - x0)) / ZR;//A矩阵里面是观测值
		A.at<float>(2 * i, 1) = (R.at<float>(0, 1)*f + R.at<float>(2, 1)*(x - x0)) / ZR;
		A.at<float>(2 * i, 2) = (R.at<float>(0, 2)*f + R.at<float>(2, 2)*(x - x0)) / ZR;
		A.at<float>(2 * i, 3) = (y - y0)*sin(Omega) - (((x - x0) / f)*((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) + f * cos(Kappa))*cos(Omega);
		A.at<float>(2 * i, 4) = -1 * f*sin(Kappa) - ((x - x0) / f)*((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
		A.at<float>(2 * i, 5) = y - y0;
		A.at<float>(2 * i, 6) = (x - x0) / f;
		A.at<float>(2 * i, 7) = 1;
		A.at<float>(2 * i, 8) = 0;
		A.at<float>(2 * i, 9) = -1 * (x - x0) * powf(r, 2);
		A.at<float>(2 * i, 10) = -1 * (x - x0) * powf(r, 4);
		A.at<float>(2 * i, 11) = -1 * 2 * (x - x0)*(y - y0);
		A.at<float>(2 * i, 12) = -1 * (powf(r, 2) + 2 * powf((x - x0), 2));

		//3.2.第二个方程（对y）
		L.at<float>(2 * i + 1, 0) = y - yi;
		//更新A矩阵
		A.at<float>(2 * i + 1, 0) = (R.at<float>(1, 0)*f + R.at<float>(2, 0)*(y - y0)) / ZR;
		A.at<float>(2 * i + 1, 1) = (R.at<float>(1, 1)*f + R.at<float>(2, 1)*(y - y0)) / ZR;
		A.at<float>(2 * i + 1, 2) = (R.at<float>(1, 2)*f + R.at<float>(2, 2)*(y - y0)) / ZR;
		A.at<float>(2 * i + 1, 3) = -1 * (x - x0)*sin(Omega) - (((y - y0) / f)*((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) - f * sin(Kappa))*cos(Omega);
		A.at<float>(2 * i + 1, 4) = -1 * f*cos(Kappa) - ((y - y0) / f)*((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
		A.at<float>(2 * i + 1, 5) = -1 * (x - x0);
		A.at<float>(2 * i + 1, 6) = (y - y0) / f;
		A.at<float>(2 * i + 1, 7) = 0;
		A.at<float>(2 * i + 1, 8) = 1;
		A.at<float>(2 * i + 1, 9) = -1 * (y - y0) * powf(r, 2);
		A.at<float>(2 * i + 1, 10) = -1 * (y - y0) * powf(r, 4);
		A.at<float>(2 * i + 1, 11) = -1 * (powf(r, 2) + 2 * powf((y - y0), 2));
		A.at<float>(2 * i + 1, 12) = -1 * 2 * (x - x0)*(y - y0);
		//显示本次迭代结果
		line(imgt, Point2i(2136 + (xi)-15, 1424 - (yi)), Point2i(2136 + (xi)+15, 1424 - (yi)), Scalar(0, 0, 255), 2, LINE_AA, 0);
		line(imgt, Point2i(2136 + (xi), 1424 - (yi)-15), Point2i(2136 + (xi), 1424 - (yi)+15), Scalar(0, 0, 255), 2, LINE_AA, 0);
		circle(imgt, Point2i(2136 + (xi), 1424 - (yi)), 15, Scalar(0, 0, 255), 1, LINE_AA, 0);
	}
	imshow("iteration", imgt);
	waitKey(1200);
}

void RESECTION::updateX()
{
	//求改正值
	X = (A.t()*A).inv()*A.t()*L;
	cout << X.at<float>(0, 0) << endl;
	//更新一下外方元素成员变量
	Xs = Xs + X.at<float>(0, 0);
	Ys = Ys + X.at<float>(1, 0);
	Zs = Zs + X.at<float>(2, 0);
	Phi = Phi + X.at<float>(3, 0);
	Omega = Omega + X.at<float>(4, 0);
	Kappa = Kappa + X.at<float>(5, 0);
	f = f + X.at<float>(6, 0);
	x0 = x0 + X.at<float>(7, 0);
	y0 = y0 + X.at<float>(8, 0);
	k1 = k1 + X.at<float>(9, 0);
	k2 = k2 + X.at<float>(10, 0);
	p1 = p1 + X.at<float>(11, 0);
	p2 = p2 + X.at<float>(12, 0);
}

void RESECTION::writeResults()//输出结果
{
	// 计算结果
	V = A * X - L;
	// 计算中误差
	Mat M0 = Mat(1, 1, CV_32FC1);
	M0 = (V.t()*V) / (float(equationNum) - float(elementNum)); //白塞尔公式
	float m = M0.at<float>(0, 0);
	m = sqrt(m);//单位权中误差
	//协因数阵
	Mat Q = Mat(elementNum, elementNum, CV_32FC1);
	Q = (A.t()*A).inv();
	//把协因数阵的对角元开方得到中误差
	Mat M = Mat(elementNum, 1, CV_32FC1);
	for (int i = 0; i < elementNum; i++)
	{
		M.at<float>(i, 0) = sqrt(Q.at<float>(i, i)*m);
	}
	string results_file_path = image_file_path;
	results_file_path.append("results.csv");
	ofstream ofs(results_file_path);
	if (!ofs)
	{
		cout << "Cannot Write Results" << endl;
		return;
	}
	ofs << "*******************************************************" << endl;
	ofs << "If all results is NaN, the notice in the line 118 of RESECTION.cpp may be a solution" << endl;
	ofs << "*******************************************************" << endl;
	ofs << "Number of iterations" << itrNum << endl;
	ofs << "MSE(mean square error) of unit weight" << m << "pixel" << endl;
	ofs << "Para Names,Results,MSE,Unit" << endl;
	ofs << "Xs," << Xs << "," << M.at<float>(0, 0) << "," << " mm" << endl;
	ofs << "Ys," << Ys << "," << M.at<float>(1, 0) << "," << " mm" << endl;
	ofs << "Zs," << Zs << "," << M.at<float>(2, 0) << "," << " mm" << endl;
	ofs << "Phi," << Phi << "," << M.at<float>(3, 0) << "," << " Rad" << endl;
	ofs << "Omega," << Omega << "," << M.at<float>(4, 0) << "," << " Rad" << endl;
	ofs << "Kappa," << Kappa << "," << M.at<float>(5, 0) << "," << " Rad" << endl;
	ofs << "f," << f << "," << M.at<float>(6, 0) << "," << " pixel" << endl;
	ofs << "x0," << x0 << "," << M.at<float>(7, 0) << "," << " pixel" << endl;
	ofs << "y0," << y0 << "," << M.at<float>(8, 0) << "," << " pixel" << endl;
	ofs << "k1," << k1 << "," << M.at<float>(9, 0) << "," << " (pixel)^(-2)" << endl;
	ofs << "k2," << k2 << "," << M.at<float>(10, 0) << "," << " (pixel)^(-4)" << endl;
	ofs << "p1," << p1 << "," << M.at<float>(11, 0) << "," << " (pixel)^(-1)" << endl;
	ofs << "p2," << p2 << "," << M.at<float>(12, 0) << "," << " (pixel)^(-1)" << endl;
	ofs << "*******************************************************" << endl;
	ofs << "Pt No.,x-res,y-res,unit is pixel" << endl;
	for (int i = 0; i < imagePtsNum; i++)
	{
		ofs << imagePts.at(i).ID << "," << V.at<float>(2 * i, 0) << "," << V.at<float>(2 * i + 1, 0) << endl;
	}
	ofs.close();
}