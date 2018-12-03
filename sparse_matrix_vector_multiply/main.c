#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "./matrix.c"

#define MATRIX_SIZE 1000
#define ITERATIONS_TO_RUN 1000

#define SPARSENESS_PERCENTAGE 50.0
#define SPARSENESS (double)(1.0 - SPARSENESS_PERCENTAGE / 100.0)

int main()
{
  printf("\n== Generating matrix ==\n");
  clock_t matrix_begin = clock();
  struct SparseMatrix matrix = generateMatrix(MATRIX_SIZE, SPARSENESS);
  clock_t matrix_end = clock();
  double matrix_time = 1000.0 * (double)(matrix_end - matrix_begin) / CLOCKS_PER_SEC;
  printf(" elements:\n");
  printf(" - total: %d\n", matrix.elementCount);
  unsigned int triangleElements = matrix.dimensions * (matrix.dimensions + 1) / 2;
  printf(" - sparseness: %.2f %%\n", 100.0 - 100.0 * (double)matrix.elementCount / (double)triangleElements);
  printf(" - non-zero: %.2f %%\n", 100.0 * (double)matrix.elementCount / (double)triangleElements);

  printf(" time:\n");
  printf(" - total: %f milliseconds\n", matrix_time);
  printf(" - per row: %f microseconds\n", 1000.0 * matrix_time / MATRIX_SIZE);

  printf("\n== Running %d iterations ==\n", ITERATIONS_TO_RUN);
  clock_t begin = clock();

  clock_t end = clock();
  double time = 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n== Computation completed! ==\n", time, 1000.0 * time / ITERATIONS_TO_RUN);
  printf(" - total time: %f milliseconds\n", time);
  printf(" - per iteration: %f microseconds\n", 1000.0 * time / ITERATIONS_TO_RUN);
}