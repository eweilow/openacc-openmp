#include <stdlib.h>
#include <time.h>

#include "./shared.c"

#define MATRIX_SIZE 10000

struct SparseMatrix
{
  unsigned int dimensions;
  float *elements;
  unsigned int elementCount;
  unsigned int *elementIndices;
  unsigned int *rowOffsets;
};

#define BATCH 128

void matrixMultiply(struct SparseMatrix matrix,
                    float *elements,
                    unsigned int *elementIndices,
                    unsigned int *rowOffsets,
                    float *inputVector,
                    float *outputVector)
{
#pragma acc parallel loop present(elements, elementIndices, rowOffsets, inputVector, outputVector)
  for (int batch = 0; batch < matrix.dimensions / BATCH; ++batch)
  {
#pragma acc loop worker
    for (int row = BATCH * batch; row < min(matrix.dimensions, BATCH * (batch + 1)), ++row)
    {
      int startOffset = rowOffsets[row];
      int endOffset = rowOffsets[row + 1];

      float sum = 0.0;
#pragma acc loop vector reduction(+ \
                                  : sum)
      for (int i = startOffset; i < endOffset; ++i)
      {
        float matrixElement = elements[i];
        int column = elementIndices[i];

        sum += matrixElement * inputVector[column];
      }
      outputVector[row] = sum;
    }
  }
}

void computeMatrixIterations(struct SparseMatrix matrix,
                             float *inputVector,
                             float *outputVector)
{
#pragma acc data copyin(matrix, matrix.elements [0:matrix.elementCount], matrix.elementIndices [0:matrix.elementCount], matrix.rowOffsets [0:matrix.dimensions + 1]) copyin(inputVector [0:matrix.dimensions]) copyin(outputVector [0:matrix.dimensions]) copyout(outputVector [0:matrix.dimensions])
  {
    for (int i = 0; i < 1000; i++)
    {
      matrixMultiply(matrix, matrix.elements, matrix.elementIndices, matrix.rowOffsets, inputVector, outputVector);
    }
  }
}

float getMatrixElement(int row, int column, int totalSize)
{
  if (row % 3 == 0)
  {
    return 1.0 / (float)totalSize;
  }
  if (column % 3 == 0)
  {
    return 2.0 / (float)totalSize;
  }
  return 0.0;
}

int main()
{
  struct SparseMatrix matrix;
  float *inputVector;
  float *outputVector;

  matrix.dimensions = MATRIX_SIZE;
  matrix.elementCount = 0;
  printf("Initializing matrices\n");

  for (int i = 0; i < matrix.dimensions; ++i)
  {
    for (int j = 0; j < matrix.dimensions; ++j)
    {
      float element = getMatrixElement(i, j, matrix.dimensions);
      if (fabsf(element) >= 1e-15f)
      {
        ++matrix.elementCount;
      }
    }
  }

  matrix.elements = (float *)malloc(sizeof(float) * matrix.elementCount);
  matrix.elementIndices = (unsigned int *)malloc(sizeof(unsigned int) * matrix.elementCount);
  matrix.rowOffsets = (unsigned int *)malloc(sizeof(unsigned int) * (matrix.dimensions + 1));

  int currentRowOffset = 0;
  for (int row = 0; row < matrix.dimensions; row++)
  {
    matrix.rowOffsets[row] = currentRowOffset;
    for (int column = 0; column < matrix.dimensions; column++)
    {
      float element = getMatrixElement(row, column, matrix.dimensions);
      if (fabsf(element) >= 1e-15f)
      {
        //printf("%d %d: %f", row, column, element);
        matrix.elementIndices[currentRowOffset] = column;
        matrix.elements[currentRowOffset] = element;
        ++currentRowOffset;
      }
    }
  }

  matrix.rowOffsets[matrix.dimensions] = currentRowOffset;

  inputVector = (float *)malloc((sizeof(float)) * matrix.dimensions);
  outputVector = (float *)malloc((sizeof(float)) * matrix.dimensions);

  printf("Initializing vectors\n");
  initializeVectors(inputVector, outputVector, matrix.dimensions);

  printf("Starting matrix multiplication\n\n");
  clock_t begin = clock();

  computeMatrixIterations(matrix, inputVector, outputVector);

  clock_t end = clock();
  printf("\nComputation took %f ms\n", 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC);

  printVector(inputVector, matrix.dimensions);
  printVector(outputVector, matrix.dimensions);
  //compareVectors(inputVector, outputVector, MATRIX_SIZE);
  return 0;
}