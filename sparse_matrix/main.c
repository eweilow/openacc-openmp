#include <stdlib.h>
#include <time.h>

#include "./shared.c"

#define MATRIX_SIZE 1000000

float *matrixElements;
unsigned int *indices;
unsigned int elementCount;
unsigned int rowOffsets[MATRIX_SIZE];

float inputVector[MATRIX_SIZE];
float outputVector[MATRIX_SIZE];

void matrixMultiply()
{
#pragma acc parallel loop copyin(matrixElements [0:elementCount], indices [0:elementCount], rowOffsets [0:MATRIX_SIZE]) \
    copyin(inputVector) copy(outputVector)
  for (int row = 0; row < MATRIX_SIZE; ++row)
  {
    int startOffset = rowOffsets[row];
    int endOffset = (row + 1 >= MATRIX_SIZE) ? MATRIX_SIZE : rowOffsets[row + 1];

    float sum = 0.0;
#pragma acc loop vector reduction(+ \
                                  : sum)
    for (int i = startOffset; i < endOffset; ++i)
    {
      float matrixElement = matrixElements[i];
      int column = indices[i];

      sum += matrixElement * inputVector[column];
    }
    outputVector[row] = sum;
  }
}

float getMatrixElement(int row, int column)
{
  if (row % 3 == 0)
  {
    return 1.0 / (float)MATRIX_SIZE;
  }
  if (column % 3 == 0)
  {
    return 2.0 / (float)MATRIX_SIZE;
  }
  return 0.0;
}

int main()
{
  printf("Initializing matrices\n");
  elementCount = 0;
#pragma acc parallel loop reduction(+ \
                                    : elementCount)
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    for (int j = 0; j < MATRIX_SIZE; j++)
    {
      float element = getMatrixElement(i, j);
      if (fabsf(element) >= 1e-15f)
      {
        ++elementCount;
      }
    }
  }

  matrixElements = (float *)malloc((sizeof(float)) * elementCount);
  indices = (unsigned int *)malloc((sizeof(unsigned int)) * elementCount);

  int currentRowOffset = 0;
  for (int row = 0; row < MATRIX_SIZE; row++)
  {
    rowOffsets[row] = currentRowOffset;
    for (int column = 0; column < MATRIX_SIZE; column++)
    {
      float element = getMatrixElement(row, column);
      if (fabsf(element) >= 1e-15f)
      {
        //printf("%d %d: %f", row, column, element);
        indices[currentRowOffset] = column;
        matrixElements[currentRowOffset] = element;
        ++currentRowOffset;
      }
    }
  }

  printf("Initializing vectors\n");
  initializeVectors(inputVector, outputVector, MATRIX_SIZE);
  printf("Starting matrix multiplication\n\n");
  clock_t begin = clock();

  matrixMultiply();

  clock_t end = clock();
  printf("\nComputation took %f ms\n", 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC);

  printVector(inputVector, MATRIX_SIZE);
  printVector(outputVector, MATRIX_SIZE);
  //compareVectors(inputVector, outputVector, MATRIX_SIZE);
  return 0;
}