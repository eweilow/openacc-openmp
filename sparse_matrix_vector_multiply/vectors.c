#include <stdlib.h>

void createVector(double **into, unsigned int length, double element)
{
  *into = (double *)malloc(sizeof(double) * length);
  for (unsigned int n = 0; n < length; ++n)
  {
    (*into)[n] = element;
  }
}

void printVector(double *vec, unsigned int length)
{
  printf("  ");
  for (unsigned int n = 0; n < length; ++n)
  {
    printf("%.4f  \t", vec[n]);
  }
  printf("\n");
}