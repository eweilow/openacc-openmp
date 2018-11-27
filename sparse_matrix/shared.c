#include <math.h>

void initializeVectors(float *a, float *b, int length)
{
  for (int i = 0; i < length; i++)
  {
    a[i] = (float)i;
    b[i] = 0.0;
  }
}

float matrixElement(int row, int column)
{
  if (row == column)
  {
    return 1.0;
  }
  return 0.0;
}

void printVector(float *vec, int length)
{
  const int standardSize = 10;
  int size = length < standardSize ? length : standardSize;
  for (int i = 0; i < size; i++)
  {
    printf("%.2f ", vec[i]);
  }
  printf("\n");
}

void compareVectors(float *a, float *b, int length)
{
  for (int i = 0; i < length; i++)
  {
    float difference = fabs(a[i] - b[i]);
    if (difference > 0.001)
    {
      printf("mismatch at index %d: %f != %f (%f)\n", i, a[i], b[i], difference);
    }
  }
  printf("\n");
}