#include <stdio.h>

int  main() {
  #pragma  omp  parallel
  {


  printf("Hello\n");
  printf("World\n");
  }
  return  0;
}
