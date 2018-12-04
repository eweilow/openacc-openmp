#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "./matrix.c"
#include "./vectors.c"
#include "./multiply.c"

#define MATRIX_SIZE 700000
#define ITERATIONS_TO_RUN 100

#define SPARSENESS_PERCENTAGE 99.99
#define SPARSENESS (double)(SPARSENESS_PERCENTAGE / 100.0)

// #define VEC_DEBUG

int main()
{
  printf("\n== Generating matrix ==\n");
  clock_t matrix_begin = clock();
  struct SparseMatrix matrix = generateMatrix(MATRIX_SIZE, SPARSENESS);
  clock_t matrix_end = clock();
  double matrix_time = 1000.0 * (double)(matrix_end - matrix_begin) / CLOCKS_PER_SEC;
  printf(" elements:\n");
  printf(" - sparseness: %.3f %%\n", 100.0 - 100.0 * (double)matrix.elementCount / (double)matrix.dimensions / (double)matrix.dimensions);
  printf(" - non-zero: %.3f %%\n", 100.0 * (double)matrix.elementCount / (double)matrix.dimensions / (double)matrix.dimensions);
  printf(" - total non-zero: %d\n", matrix.elementCount);
  printf(" - wanted non-zero: %d\n", (unsigned long)((long double)(1.0 - SPARSENESS) * (double)matrix.dimensions * matrix.dimensions));

  printf(" time:\n");
  printf(" - total: %f milliseconds\n", matrix_time);
  printf(" - per row: %f microseconds\n", 1000.0 * matrix_time / MATRIX_SIZE);

  printf("\n== Generating vectors ==\n");
  double *input, *output;
  createVector(&input, MATRIX_SIZE, 1.0);
  createVector(&output, MATRIX_SIZE, 0.0);

  printf("\n== Running %d iterations ==\n", ITERATIONS_TO_RUN);
  clock_t begin = clock();

  matrixIterate(&matrix, input, output, ITERATIONS_TO_RUN);

  clock_t end = clock();
  double time = 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n== Computation completed! ==\n", time, 1000.0 * time / ITERATIONS_TO_RUN);
  printf(" - total time: %f milliseconds\n", time);
  printf(" - per iteration: %f microseconds (%.4f milliseconds)\n", 1000.0 * time / ITERATIONS_TO_RUN, time / ITERATIONS_TO_RUN);

#ifdef VEC_DEBUG
  printf("\n== Results ==\n");
  printf(" in:\n");
  free(input);
  createVector(&input, MATRIX_SIZE, 1.0);
  printVector(input, MATRIX_SIZE);

  printf(" out:\n");
  printVector(output, MATRIX_SIZE);
#endif
}