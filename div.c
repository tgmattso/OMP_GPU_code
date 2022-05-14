/*

NAME:
   hist:  div

Purpose:
   This program is designed to play around with divergent execution
   flow on a GPU.   I fill an array of type TYPE with their index values.
   Then using a random number generator, randomly select a fraction 
   of array values that I set to a negative value.  Then I assign corresponding
   elements of a second array (initialized to zero) to the square os each 
   element of the x array that is positive.

Usage:
   To keep the program as simple as possible, you must edit the file
   and change basic parameters.  Then compile and run the program.

History: 
   Written by Tim Mattson, 5/2022.

*/
#include <stdio.h>
#ifdef APPLE
#include <stdlib.h>
#endif
#include <omp.h>
#include <math.h>
#include "random.h"

// uncomment this #define if you want tons of diagnostic output
//#define     DEBUG         0

#define     TYPE             float
#define     num_trials     1000000 // number of x values
#define     DEF_FRAC   0.05    // default fraction of values to negate
#define     A                    5.0      // Multiplier of X squared
#define     B                   3.0       // Multiplier of X
#define     C                   3.14    // constant term

static TYPE xlow        = 0.0;      // low end of x range
static TYPE xhi         = 1.0;    // High end of x range
static TYPE TOL       = 0.00001;

int main(int argc, char **argv)
{

    TYPE x[num_trials], y[num_trials];     // array used to assign counters in the histogram 
    double time, sum1, sum2;
    TYPE frac = DEF_FRAC;
    
    if(argc ==2){
      frac = (TYPE) atof(argv[1]);
      if((frac>1.0) || (frac<=0.0)) frac=DEF_FRAC;
    }



   seed(xlow, xhi);  // seed the random number generator over range of x

   // fill the arrays
   for(int i=0;i<num_trials;i++){
     y[i] = (TYPE)0.0;
     x[i] = (TYPE)i;
     if (drandom() < frac) x[i] = (-1.0)*x[i];
   }

   printf(" Quadratic model %f, %f, %f WITH if statement in the loop\n", (float)A,(float)B,(float)C);
   time = omp_get_wtime();
   
   // square positive elements of the array
   for(int i=0;i<num_trials;i++){
    
      if(x[i] > (TYPE)0.0) y[i] = (A*x[i]*x[i] + B*x[i] +C);
     
      #ifdef DEBUG
      printf("i = %d,  xi = %f, yi= %f frac = %f\n",i,(float)x[i], (float)y[i],  (float)frac);
      #endif

   }
   printf("%d numbers squared in %f secs\n",num_trials,omp_get_wtime()-time);



   printf(" Quadratic model %f, %f, %f WITHOUT if statement in the loop\n", (float)A,(float)B,(float)C);
   
   sum1 = 0.0;
   for(int i=0; i<num_trials;i++){
        sum1 += y[i];
        y[i] = (TYPE) 0.0;
   }
   
   time = omp_get_wtime();
   
   // square positive elements of the array
   for(int i=0;i<num_trials;i++){
    
   y[i] = ((A*x[i]*x[i] + B*x[i] +C))*(x[i]>(0.0));
     
      #ifdef DEBUG
      printf("i = %d,  xi = %f, yi= %f frac = %f, %d \n",i,(float)x[i], (float)y[i],  (float)frac,(x[i]>((TYPE)0.0)));
      #endif

   }
   
   sum2 = 0.0;
   for(int i=0; i<num_trials;i++){
        sum2 += y[i];
        y[i] = (TYPE) 0.0;
   }
   
   printf("%d numbers in quadratic model in %f secs.  \n If correct then %f == %f\n",num_trials,omp_get_wtime()-time, (float)sum1, (float)sum2);



   return 0;
}
	  
