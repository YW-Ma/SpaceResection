//
// File: mldivide.cpp
//
// MATLAB Coder version            : 4.0
// C/C++ source code generated on  : 23-Sep-2018 20:19:47
//

// Include Files
#include <cmath>
#include "rt_nonfinite.h"
#include "getMatrixA.h"
#include "mldivide.h"
#include "getMatrixA_emxutil.h"
#include "xgeqp3.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *A
//                const double B[3]
//                emxArray_real_T *Y
// Return Type  : void
//
void mldivide(const emxArray_real_T *A, const double B[3], emxArray_real_T *Y)
{
  emxArray_real_T *b_A;
  int i1;
  int minmn;
  emxArray_int32_T *jpvt;
  double tau_data[3];
  int tau_size[1];
  int rankR;
  int j;
  signed char ipiv[3];
  int maxmn;
  int c;
  double b_B[3];
  int ix;
  double tol;
  double s;
  int ijA;
  if (A->size[1] == 0) {
    i1 = Y->size[0];
    Y->size[0] = 0;
    emxEnsureCapacity_real_T(Y, i1);
  } else {
    emxInit_real_T(&b_A, 2);
    if (3 == A->size[1]) {
      i1 = b_A->size[0] * b_A->size[1];
      b_A->size[0] = A->size[0];
      b_A->size[1] = A->size[1];
      emxEnsureCapacity_real_T1(b_A, i1);
      minmn = A->size[0] * A->size[1];
      for (i1 = 0; i1 < minmn; i1++) {
        b_A->data[i1] = A->data[i1];
      }

      for (i1 = 0; i1 < 3; i1++) {
        ipiv[i1] = (signed char)(1 + i1);
      }

      for (j = 0; j < 2; j++) {
        c = j << 2;
        minmn = 0;
        ix = c;
        tol = std::abs(b_A->data[c]);
        for (rankR = 2; rankR <= 3 - j; rankR++) {
          ix++;
          s = std::abs(b_A->data[ix]);
          if (s > tol) {
            minmn = rankR - 1;
            tol = s;
          }
        }

        if (b_A->data[c + minmn] != 0.0) {
          if (minmn != 0) {
            ipiv[j] = (signed char)((j + minmn) + 1);
            ix = j;
            minmn += j;
            for (rankR = 0; rankR < 3; rankR++) {
              tol = b_A->data[ix];
              b_A->data[ix] = b_A->data[minmn];
              b_A->data[minmn] = tol;
              ix += 3;
              minmn += 3;
            }
          }

          i1 = (c - j) + 3;
          for (maxmn = c + 1; maxmn < i1; maxmn++) {
            b_A->data[maxmn] /= b_A->data[c];
          }
        }

        minmn = c;
        maxmn = c + 3;
        for (rankR = 1; rankR <= 2 - j; rankR++) {
          tol = b_A->data[maxmn];
          if (b_A->data[maxmn] != 0.0) {
            ix = c + 1;
            i1 = (minmn - j) + 6;
            for (ijA = 4 + minmn; ijA < i1; ijA++) {
              b_A->data[ijA] += b_A->data[ix] * -tol;
              ix++;
            }
          }

          maxmn += 3;
          minmn += 3;
        }
      }

      for (maxmn = 0; maxmn < 3; maxmn++) {
        b_B[maxmn] = B[maxmn];
      }

      for (minmn = 0; minmn < 2; minmn++) {
        if (ipiv[minmn] != minmn + 1) {
          tol = b_B[minmn];
          b_B[minmn] = b_B[ipiv[minmn] - 1];
          b_B[ipiv[minmn] - 1] = tol;
        }
      }

      for (rankR = 0; rankR < 3; rankR++) {
        minmn = 3 * rankR;
        if (b_B[rankR] != 0.0) {
          for (maxmn = rankR + 1; maxmn + 1 < 4; maxmn++) {
            b_B[maxmn] -= b_B[rankR] * b_A->data[maxmn + minmn];
          }
        }
      }

      for (rankR = 2; rankR >= 0; rankR--) {
        minmn = 3 * rankR;
        if (b_B[rankR] != 0.0) {
          b_B[rankR] /= b_A->data[rankR + minmn];
          for (maxmn = 0; maxmn < rankR; maxmn++) {
            b_B[maxmn] -= b_B[rankR] * b_A->data[maxmn + minmn];
          }
        }
      }

      i1 = Y->size[0];
      Y->size[0] = 3;
      emxEnsureCapacity_real_T(Y, i1);
      for (i1 = 0; i1 < 3; i1++) {
        Y->data[i1] = b_B[i1];
      }
    } else {
      i1 = b_A->size[0] * b_A->size[1];
      b_A->size[0] = A->size[0];
      b_A->size[1] = A->size[1];
      emxEnsureCapacity_real_T1(b_A, i1);
      minmn = A->size[0] * A->size[1];
      for (i1 = 0; i1 < minmn; i1++) {
        b_A->data[i1] = A->data[i1];
      }

      emxInit_int32_T(&jpvt, 2);
      xgeqp3(b_A, tau_data, tau_size, jpvt);
      rankR = 0;
      if (3 < b_A->size[1]) {
        minmn = 3;
        maxmn = b_A->size[1];
      } else {
        minmn = b_A->size[1];
        maxmn = 3;
      }

      if (minmn > 0) {
        tol = (double)maxmn * std::abs(b_A->data[0]) * 2.2204460492503131E-16;
        while ((rankR < minmn) && (!(std::abs(b_A->data[rankR + b_A->size[0] *
                  rankR]) <= tol))) {
          rankR++;
        }
      }

      for (maxmn = 0; maxmn < 3; maxmn++) {
        b_B[maxmn] = B[maxmn];
      }

      minmn = b_A->size[1];
      i1 = Y->size[0];
      Y->size[0] = minmn;
      emxEnsureCapacity_real_T(Y, i1);
      for (i1 = 0; i1 < minmn; i1++) {
        Y->data[i1] = 0.0;
      }

      minmn = b_A->size[1];
      if (3 < minmn) {
        minmn = 3;
      }

      for (j = 0; j < minmn; j++) {
        if (tau_data[j] != 0.0) {
          tol = b_B[j];
          for (maxmn = j + 1; maxmn < 3; maxmn++) {
            tol += b_A->data[maxmn + b_A->size[0] * j] * b_B[maxmn];
          }

          tol *= tau_data[j];
          if (tol != 0.0) {
            b_B[j] -= tol;
            for (maxmn = j + 1; maxmn < 3; maxmn++) {
              b_B[maxmn] -= b_A->data[maxmn + b_A->size[0] * j] * tol;
            }
          }
        }
      }

      for (maxmn = 0; maxmn < rankR; maxmn++) {
        Y->data[jpvt->data[maxmn] - 1] = b_B[maxmn];
      }

      for (j = rankR - 1; j + 1 > 0; j--) {
        Y->data[jpvt->data[j] - 1] /= b_A->data[j + b_A->size[0] * j];
        for (maxmn = 0; maxmn < j; maxmn++) {
          Y->data[jpvt->data[maxmn] - 1] -= Y->data[jpvt->data[j] - 1] *
            b_A->data[maxmn + b_A->size[0] * j];
        }
      }

      emxFree_int32_T(&jpvt);
    }

    emxFree_real_T(&b_A);
  }
}

//
// File trailer for mldivide.cpp
//
// [EOF]
//
