#include <time.h>
#include <math.h>

#include "./shared.c"

#define ITERATIONS_TO_RUN 1

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
      printf("%d %d\n", i, j);
    }
  }
}

int main()
{
  printf("Initializing vectors\n");
  initializeVectors(inputVector, outputVector);
  printf("Starting matrix multiplication\n");
  clock_t begin = clock();

#pragma acc data copy(outputVector) copyin(inputVector)
  for (int n = 0; n < ITERATIONS_TO_RUN; n++)
  {
    matrixMultiply();
  }

  clock_t end = clock();
  double time = 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n== Computation completed! ==\n", time, 1000.0 * time / ITERATIONS_TO_RUN);
  printf(" - total time: %f milliseconds\n", time);
  printf(" - per iteration: %f microseconds (%.4f milliseconds)\n", 1000.0 * time / ITERATIONS_TO_RUN, time / ITERATIONS_TO_RUN);

  // printf("Input: ");
  // printVector(inputVector);
  // printf("Output: ");
  // printVector(outputVector);
  // compareVectors(inputVector, outputVector);
  // printf("\n\n");

  return 0;
}