//
// File: getMatrixA_types.h
//
// MATLAB Coder version            : 4.0
// C/C++ source code generated on  : 23-Sep-2018 20:19:47
//
#ifndef GETMATRIXA_TYPES_H
#define GETMATRIXA_TYPES_H

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_int32_T

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T
#endif

//
// File trailer for getMatrixA_types.h
//
// [EOF]
//
