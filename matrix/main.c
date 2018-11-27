#include <time.h>
#include <math.h>

#include "./shared.c"

#define BLOCK_SIZE 200000
float inputVector[MATRIX_SIZE];
float outputVector[MATRIX_SIZE];

void multiplyMatrixBlock(int fromRow, int toRow, int fromColumn, int toColumn)
{
#pragma acc parallel loop present(outputVector, inputVector)
  for (int row = fromRow; row < toRow; ++row)
  {
    float sum = 0.0;
#pragma acc loop vector reduction(+ \
                                  : sum)
    for (int column = fromColumn; column < toColumn; ++column)
    {
      sum += matrixElement(row, column) * inputVector[column];
    }
    outputVector[row] += sum;
  }
}

void matrixMultiply()
{
#pragma acc data copy(outputVector) copyin(inputVector)
  for (int i = 0; i <= MATRIX_SIZE / BLOCK_SIZE; i++)
  {
    int startI = i * BLOCK_SIZE;
    int endI = (i + 1) * BLOCK_SIZE;
    if (endI >= MATRIX_SIZE)
    {
      endI = MATRIX_SIZE;
    }
    for (int j = 0; j <= MATRIX_SIZE / BLOCK_SIZE; j++)
    {
      int startJ = j * BLOCK_SIZE;
      int endJ = (j + 1) * BLOCK_SIZE;
      if (endJ >= MATRIX_SIZE)
      {
        endJ = MATRIX_SIZE;
      }
      multiplyMatrixBlock(startI, endI, startJ, endJ);
    }
  }
}

int main()
{
  printf("Initializing vectors\n");
  initializeVectors(inputVector, outputVector);
  printf("Starting matrix multiplication\n");
  clock_t begin = clock();

  matrixMultiply();

  clock_t end = clock();
  printf("\n\nComputation took %f ms\n", 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC);
  printf("Input: ");
  printVector(inputVector);
  printf("Output: ");
  printVector(outputVector);
  compareVectors(inputVector, outputVector);
  printf("\n\n");

  return 0;
}