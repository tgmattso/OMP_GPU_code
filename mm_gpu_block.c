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

  /* Team local arrays */
  TYPE Awrk[Bsize*Bsize];
  TYPE Bwrk[Bsize*Bsize];

 #pragma omp target map(tofrom:C[0:Ndim*Mdim]) map(to:B[0:Pdim*Mdim],A[0:Ndim*Pdim])
 #pragma omp teams distribute collapse(2) num_teams(Nblk*Mblk) allocate(omp_pteam_mem_alloc: Awrk, Bwrk) private(Awrk, Bwrk)
 for (ib=0; ib < Nblk; ib++){ /* Loop over blocks of C. One team per block */
    for (jb=0; jb < Mblk; jb++){

      for (kb=0; kb<Pblk; kb++){

        /* Copy block of A into pteam memory */
        #pragma omp parallel for collapse(2)
        for (i=ib*Bsize; i<((ib+1)*Bsize); i++){
          for(k=kb*Bsize; k<((kb+1)*Bsize); k++){
            Awrk[(i%Bsize)*Bsize + (k%Bsize)] = A[i*Pdim+k];
          }
        }

        /* Copy block of B into pteam memory, transposing */
        #pragma omp parallel for collapse(2)
        for (j=jb*Bsize; j<((jb+1)*Bsize); j++){
          for(k=kb*Bsize; k<((kb+1)*Bsize); k++){
            Bwrk[(j%Bsize)*Bsize + (k%Bsize)] = B[k*Mdim+j];
          }
        }


        /* matrix multiply block */
        #pragma omp parallel for collapse(2)
        for (i=ib*Bsize; i<((ib+1)*Bsize); i++){
          for (j=jb*Bsize; j<((jb+1)*Bsize); j++){
            for(k=kb*Bsize; k<((kb+1)*Bsize); k++){
              /* C(i,j) = sum(over k) A(i,k) * B(k,j) */
              *(C+(i*Mdim+j)) += Awrk[(i%Bsize)*Bsize + (k%Bsize)] * Bwrk[(j%Bsize)*Bsize + (k%Bsize)];
            }
          }
        }

      }
    }
  }
}
