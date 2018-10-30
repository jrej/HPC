#include  <stdio.h>
#include  <omp.h>
#define  SIZE  100
#define  CHUNK  10
int  main () {
double a[SIZE], b[SIZE], c[SIZE], d[SIZE];
for (int i = 0; i < SIZE; i++) {
a[i] = i;
b[i] = i;
}
#pragma  omp  parallel
{
#pragma  omp  for  schedule(static) nowait
for (int i = 0; i < SIZE; i += 1) {
c[i] = a[i] + b[i];
}
#pragma  omp  for  schedule(guided)
for (int i = 0; i < SIZE; i++) {
d[i] = a[i] + c[i];
}
}
for (int i = 0; i < SIZE; i += 1) {
printf("%g ", d[i]);
}
printf("\n");
return  0;
}
