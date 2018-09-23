//
// File: main.cpp
//
// MATLAB Coder version            : 4.0
// C/C++ source code generated on  : 23-Sep-2018 20:19:47
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "getMatrixA.h"
#include "main.h"
#include "getMatrixA_terminate.h"
#include "getMatrixA_emxAPI.h"
#include "getMatrixA_initialize.h"

// Function Declarations
static emxArray_real_T *argInit_Unboundedx1_real_T();
static double argInit_real_T();
static emxArray_real_T *c_argInit_UnboundedxUnbounded_r();
static void main_getMatrixA();

// Function Definitions

//
// Arguments    : void
// Return Type  : emxArray_real_T *
//
static emxArray_real_T *argInit_Unboundedx1_real_T()
{
  emxArray_real_T *result;
  static int iv1[1] = { 2 };

  int idx0;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real_T(1, iv1);

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result->data[idx0] = argInit_real_T();
  }

  return result;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : emxArray_real_T *
//
static emxArray_real_T *c_argInit_UnboundedxUnbounded_r()
{
  emxArray_real_T *result;
  static int iv0[2] = { 2, 2 };

  int idx0;
  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real_T(2, iv0);

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result->data[idx0 + result->size[0] * idx1] = argInit_real_T();
    }
  }

  return result;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_getMatrixA()
{
  emxArray_real_T *XYZ_;
  double X;
  double Y;
  double Z;
  double x;
  double y;
  emxArray_real_T *R;
  double fk;
  double x0;
  double b_y0;
  emxArray_real_T *X0;
  double A[12];
  emxInitArray_real_T(&XYZ_, 1);

  // Initialize function 'getMatrixA' input arguments.
  X = argInit_real_T();
  Y = argInit_real_T();
  Z = argInit_real_T();
  x = argInit_real_T();
  y = argInit_real_T();

  // Initialize function input argument 'R'.
  R = c_argInit_UnboundedxUnbounded_r();
  fk = argInit_real_T();
  x0 = argInit_real_T();
  b_y0 = argInit_real_T();

  // Initialize function input argument 'X0'.
  X0 = argInit_Unboundedx1_real_T();

  // Call the entry-point 'getMatrixA'.
  getMatrixA(X, Y, Z, x, y, R, fk, x0, b_y0, X0, A, XYZ_);
  emxDestroyArray_real_T(XYZ_);
  emxDestroyArray_real_T(X0);
  emxDestroyArray_real_T(R);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  getMatrixA_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_getMatrixA();

  // Terminate the application.
  // You do not need to do this more than one time.
  getMatrixA_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
