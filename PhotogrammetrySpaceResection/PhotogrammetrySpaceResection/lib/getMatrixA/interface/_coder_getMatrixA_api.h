/*
 * File: _coder_getMatrixA_api.h
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 23-Sep-2018 20:19:47
 */

#ifndef _CODER_GETMATRIXA_API_H
#define _CODER_GETMATRIXA_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_getMatrixA_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void getMatrixA(real_T X, real_T Y, real_T Z, real_T x, real_T y,
  emxArray_real_T *R, real_T fk, real_T x0, real_T b_y0, emxArray_real_T *X0,
  real_T A[12], emxArray_real_T *XYZ_);
extern void getMatrixA_api(const mxArray * const prhs[10], int32_T nlhs, const
  mxArray *plhs[2]);
extern void getMatrixA_atexit(void);
extern void getMatrixA_initialize(void);
extern void getMatrixA_terminate(void);
extern void getMatrixA_xil_terminate(void);

#endif

/*
 * File trailer for _coder_getMatrixA_api.h
 *
 * [EOF]
 */
