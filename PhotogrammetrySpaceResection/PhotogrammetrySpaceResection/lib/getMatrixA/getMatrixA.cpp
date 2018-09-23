#include <cmath>
#include "rt_nonfinite.h"
#include "getMatrixA.h"
#include "mldivide.h"

// (X,Y,Z)->gPts;(x,y)->imgPts;
// 1.initialize assistant values X_,Y_ Z_
// Arguments    : double X
//                double Y
//                double Z
//                double x
//                double y
//                const emxArray_real_T *R
//                double fk
//                double x0
//                double b_y0
//                const emxArray_real_T *X0
//                double A[12]
//                emxArray_real_T *XYZ_

void getMatrixA(double X, double Y, double Z, double x, double y, const
                emxArray_real_T *R, double fk, double x0, double b_y0, const
                emxArray_real_T *X0, double A[12], emxArray_real_T *XYZ_)
{
  double b_X[3];
  int i0;
  double c_X[3];
  b_X[0] = X - X0->data[0];
  b_X[1] = Y - X0->data[1];
  b_X[2] = Z - X0->data[2];
  for (i0 = 0; i0 < 3; i0++) {
    c_X[i0] = b_X[i0];
  }

  mldivide(R, c_X, XYZ_);

  // 2.calculate Marix A [only exterior orientation elements is needed.
  //                      So size of A is (2,6) instead of (2,9)]
  A[0] = 1.0 / XYZ_->data[2] * (R->data[0] * fk + R->data[R->size[0] << 1] * (x - x0));
  A[2] = 1.0 / XYZ_->data[2] * (R->data[1] * fk + R->data[1 + (R->size[0] << 1)] * (x - x0));
  A[4] = 1.0 / XYZ_->data[2] * (R->data[2] * fk + R->data[2 + (R->size[0] << 1)] * (x - x0));
  A[6] = (y - b_y0) * std::sin(X0->data[4]) - ((x - x0) / fk * ((x - x0) * std::cos(X0->data[5]) - (y - b_y0) * std::sin(X0->data[5])) + fk * std::cos(X0->data[5])) * std::cos(X0->data[4]);
  A[8] = -fk * std::sin(X0->data[5]) - (x - x0) / fk * ((x - x0) * std::sin(X0->data[5]) + (y - b_y0) * std::cos(X0->data[5]));
  A[10] = y - b_y0;
  A[1] = 1.0 / XYZ_->data[2] * (R->data[R->size[0]] * fk + R->data[R->size[0] << 1] * (y - b_y0));
  A[3] = 1.0 / XYZ_->data[2] * (R->data[1 + R->size[0]] * fk + R->data[1 + (R->size[0] << 1)] * (y - b_y0));
  A[5] = 1.0 / XYZ_->data[2] * (R->data[2 + R->size[0]] * fk + R->data[2 + (R->size[0] << 1)] * (y - b_y0));
  A[7] = (x - x0) * std::sin(X0->data[4]) - ((y - b_y0) / fk * ((x - x0) * std::cos(X0->data[5]) - (y - b_y0) * std::sin(X0->data[5])) + fk * std::sin(X0->data[5])) * std::cos(X0->data[4]);
  A[9] = -fk * std::cos(X0->data[5]) - (y - b_y0) / fk * ((x - x0) * std::sin(X0->data[5]) + (y - b_y0) * std::cos(X0->data[5]));
  A[11] = -(x - x0);
}
