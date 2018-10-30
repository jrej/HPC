//Algortithm Frame Difference
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include "scalar_matrix/include/nrutil.h"


int *Et;
int *Ot;
void frameDiff(int* It, int * It1 , int taille, int teta ,int*Et, int *Ot ){

   int x;


   for(x = 0 ; x < taille; x++ ){

     Ot[x]  =   It[x] - It1[x];
     if(Ot[x]<0)
      Ot[x] = -1 * Ot[x];
   }

   for(x=0;x<taille;x++){
      if(Ot[x] < teta)
        Et[x] = 0
      else
        Et[x] = 1


   }



}

int main(){


}
