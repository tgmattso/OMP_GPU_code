/*
**  function: Matrix Multiplication ... three loop, ikj case
**            where ijk defines the order of the loops
**
**  HISTORY: Written by Tom Deakin, January 2021.
*/
#include "mm_utils.h"

#include <assert.h>

void mm_gpu_block(int Ndim, int Mdim, int Pdim, TYPE *A, TYPE *B, TYPE *C){
  int i, j, k;
  const int Bsize = 16;
  int ib, jb, kb;

  /* Block size, must evenly divide the matrices */
  assert(Ndim % Bsize == 0);
  assert(Mdim % Bsize == 0);
  assert(Pdim % Bsize == 0);

  /* Number of blocks in each dimension */
  int Nblk = Ndim / Bsize;
  int Mblk = Mdim / Bsize;
  int Pblk = Pdim / Bsize;


// #pragma omp target map(tofrom:C[0:Ndim*Mdim]) map(to:B[0:Pdim*Mdim],A[0:Ndim*Pdim])
// #pragma omp teams distribute parallel for collapse(2)
  for (ib=0; ib < Nblk; ib++){
    for (i=ib*Bsize; i<((ib+1)*Bsize); i++){
     for (jb=0; jb<Mblk; jb++){
       for (j=jb*Bsize; j<((jb+1)*Bsize); j++){
        for (kb=0; kb<Pblk; kb++){
	  for(k=kb*Bsize; k<((kb+1)*Bsize); k++){
	   /* C(i,j) = sum(over k) A(i,k) * B(k,j) */
	   *(C+(i*Mdim+j)) += *(A+(i*Pdim+k)) *  *(B+(k*Mdim+j));
          }
	}
       }
     }
   }
  }
}
