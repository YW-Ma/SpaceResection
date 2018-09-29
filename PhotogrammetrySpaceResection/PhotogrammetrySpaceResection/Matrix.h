#pragma once
#include <iostream>
#include <cmath>
#include <ctime>

class Matrix
{
public:
	double* data;
	int row;
	int col;
private:
	int N;
public:
	//除了乘法，都是原地转换，因为乘法会产生新的大小的矩阵
	Matrix(int row, int col);//列优先
	~Matrix();
	void LUP_inverse(Matrix& Inv);//LU分解求逆
	void multiply(Matrix& multiplierB, Matrix& result);//This*B=result
	void add(Matrix& B);
	void minus(Matrix& B);

	double avg();
	void trans();//转置
	void show();
	void rand_init(int scale);
	void clone(Matrix& B);
	void set_zero();
private:
	//LUP分解
	void LUP_Descomposition(double *A, double *L, double *U, int *P);
	//LUP求解方程
	double * LUP_Solve(double *L, double *U, int *P, double *b);
	//矩阵乘法
	double * mul(double *A, double *B);
	/*****************矩阵原地转置BEGIN********************/

	/* 后继 */
	int getNext(int i, int m, int n);

	/* 前驱 */
	int getPre(int i, int m, int n);

	/* 处理以下标i为起点的环 */
	void movedata(double *mtx, int i, int m, int n);
	/* 转置，即循环处理所有环 */
	void transpose(double *mtx, int m, int n);
	/*****************矩阵原地转置END********************/

	//LUP求逆(将每列b求出的各列x进行组装)
	double * LUP_solve_inverse(double *A);
};

