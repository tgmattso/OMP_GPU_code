/*
**  function: Matrix Multiplication ... using cuBLAS
**
**  HISTORY: Written by Tom Deakin, July 2022.
*/
#include "mm_utils.h"

#ifdef USE_CUBLAS

#include <cuda_runtime.h>
#include "cublas_v2.h"

void mm_cublas(int Ndim, int Mdim, int Pdim, TYPE *A, TYPE *B, TYPE *C){
  // Initalize cuBLAS Library
  cublasHandle_t handle;
  cublasStatus_t err;

  err = cublasCreate(&handle);
  if (err != CUBLAS_STATUS_SUCCESS) {
    printf("cuBLAS Create error\n");
    return;
  }

  // Target data region to map and convert arrays
  #pragma omp target data map(tofrom:C[0:Ndim*Mdim]) map(to:B[0:Pdim*Mdim],A[0:Ndim*Pdim]) use_device_ptr(A, B, C)
  {

  // Call dgemm from cuBLAS
  // Note cuBLAS assumes column-major matrices, so we swap the order of arguments to avoid an explicit transpose
  TYPE alpha = 1.0;
  TYPE beta = 0.0;

  if (sizeof(TYPE) == sizeof(double)) {
    err = cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
      Mdim, Ndim, Pdim,
      &alpha,
      B, Mdim,
      A, Pdim,
      &beta,
      C, Mdim);
  }
  else {
    printf("Only implemented for TYPE == double\n");
    return;
  }

  if (err != CUBLAS_STATUS_SUCCESS) {
    printf("cuBLAS Dgemm error\n");
  }

  // Synchronize cuBLAS call
  cudaDeviceSynchronize();

  } // end target data region

  cublasDestroy(handle);
}

#else
void mm_cublas(int Ndim, int Mdim, int Pdim, TYPE *A, TYPE *B, TYPE *C){ }
#endif

