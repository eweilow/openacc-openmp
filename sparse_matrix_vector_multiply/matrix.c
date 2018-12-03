#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct SparseMatrix
{
  unsigned int dimensions;
  double *elements;
  unsigned int elementCount;
  unsigned int *elementColumns;
  unsigned int *rowOffsets;
};

double createMatrixElement(unsigned int randomValue, long row, long column)
{
  return (randomValue / (double)RAND_MAX) * 2.0 - 1.0;
}

void printMatrixInfo(
    unsigned int dimensions,
    double sparseness,
    unsigned int bytes,
    unsigned int allocationPerRow,
    unsigned int expectedCountPerRow,
    unsigned int expectedAllocation,
    unsigned int expectedElements)
{
  printf(" matrix info:\n");
  printf(" - dimensions: 10^%d x 10^%d (%d x %d)\n", (unsigned int)log10(dimensions), (unsigned int)log10(dimensions), dimensions, dimensions);
  printf(" - sparseness: %.2f%%\n", sparseness * 100.0);

  printf(" memory info:\n");
  printf(" - 2^%d (%d) elements per row\n", (int)ceil(log2((double)allocationPerRow)), expectedCountPerRow);
  printf(" - 2^%d (%d) elements in total\n", (int)ceil(log2((double)expectedAllocation)), expectedElements);
  printf(" - %.2f MB necessary\n", (double)bytes / 1e6);
}

// generate N random values into randomValues
void generateRandomValues(unsigned int *randomValues, unsigned int N)
{
  for (unsigned int i = 0; i < N; i++)
  {
    randomValues[i] = rand();
  }
}

// generate N random steps into randomSteps, with average distance period
void createRandomSteps(unsigned int *randomSteps, unsigned int N, unsigned int period)
{
  generateRandomValues(randomSteps, N);
  for (unsigned int i = 0; i < N; i++)
  {
    double rnd = randomSteps[i] / ((double)RAND_MAX);

    randomSteps[i] = (unsigned int)(3.0 * rnd * period);
  }
}

bool indexMatrix(struct SparseMatrix *matrix, unsigned int row, unsigned int column, double *out)
{
  //if (row > column)
  //{
  //  return indexMatrix(matrix, column, row, out);
  //}
  unsigned int start = matrix->rowOffsets[row];
  unsigned int end = matrix->rowOffsets[row + 1];
  for (unsigned int index = start; index < end; ++index)
  {
    if (matrix->elementColumns[index] == column)
    {
      *out = matrix->elements[index];
      return true;
    }
  }
  return false;
}

#define DEBUG

/*
 * The algorithm works as follows:
 *  1. 
 * 
 */

// Generate a sparse matrix
struct SparseMatrix generateMatrix(unsigned int dimensions, double sparseness)
{
  // The expected amount of elements per row, based on sparseness
  unsigned int expectedCountPerRow = (unsigned int)((double)dimensions * sparseness);
  // The amount of elements rounded up to the nearest power of 2 that is guaranteed to fit the elements in each row
  unsigned int allocationPerRow = (unsigned int)pow(2, ceil(log2((double)expectedCountPerRow)));

  struct SparseMatrix matrix;
  matrix.dimensions = dimensions;

  // The expected amount of elements necessary to allocate
  unsigned int expectedElements = sparseness * dimensions * (dimensions + 1) / 2;
  // The amount of elements necessary to allocate, rounded up to a power of 2
  unsigned int expectedAllocation = (unsigned int)pow(2, ceil(log2((double)expectedElements)));

  // Just used to print information
  unsigned int bytes = expectedAllocation * (sizeof(double) + sizeof(unsigned int)) + (dimensions + 1) * sizeof(unsigned int) + allocationPerRow * (2 * sizeof(unsigned int));
  printMatrixInfo(
      dimensions,
      sparseness,
      bytes,
      allocationPerRow,
      expectedCountPerRow,
      expectedAllocation,
      expectedElements);

  matrix.elements = (double *)malloc(sizeof(double) * expectedAllocation);
  matrix.elementColumns = (unsigned int *)malloc(sizeof(unsigned int) * expectedAllocation);
  matrix.rowOffsets = (unsigned int *)malloc(sizeof(unsigned int) * (dimensions + 1));
  matrix.elementCount = 0;

  unsigned int randomStepAverageDistance = (unsigned int)(1.0 / sparseness);
  // Used to increment column in "random" steps
  unsigned int *randomSteps = (unsigned int *)malloc(sizeof(unsigned int) * (allocationPerRow / randomStepAverageDistance));
  // Used to generate matrix elements
  unsigned int *randomValues = (unsigned int *)malloc(sizeof(unsigned int) * allocationPerRow);

  int totalRandomSteps = allocationPerRow / randomStepAverageDistance;
  createRandomSteps(randomSteps, totalRandomSteps, randomStepAverageDistance);
  int current = 0;
  for (int n = 0; n < totalRandomSteps; n++)
  {
    printf("%d (%d)\n", current, randomSteps[n] + 1);
    current += randomSteps[n] + 1;
  }
  printf("END: %d, %d\n", current, allocationPerRow);
  return matrix;

  matrix.rowOffsets[0] = 0;
  for (unsigned int row = 0; row < dimensions; ++row)
  {
    createRandomSteps(randomSteps, allocationPerRow, randomStepAverageDistance);
    generateRandomValues(randomValues, allocationPerRow);

    unsigned int index = 0;
    unsigned int elementsOfRow = 0;
    for (unsigned int column = row + randomSteps[0]; column < dimensions; column += randomSteps[++index])
    {
      if (++elementsOfRow >= expectedCountPerRow)
      {
        break;
      }
      if (matrix.elementCount >= expectedElements)
      {
        break;
      }
      unsigned int indexInMatrix = matrix.elementCount++;
      matrix.elements[indexInMatrix] = createMatrixElement(randomValues[index], row, column);
      matrix.elementColumns[indexInMatrix] = column;
    }
    matrix.rowOffsets[row + 1] = matrix.elementCount;
  }
  matrix.rowOffsets[0] = 0;

#ifdef DEBUG
  printf(" rows:\n");
  for (unsigned int el = 0; el <= dimensions; el++)
  {
    printf("  %d\n", matrix.rowOffsets[el]);
  }

  printf(" elements:\n");
  for (unsigned int el = 0; el < matrix.elementCount; el++)
  {
    printf("  %d: %.4f\n", matrix.elementColumns[el], matrix.elements[el]);
  }

  printf(" matrix:\n");
  for (unsigned int row = 0; row < dimensions; row++)
  {
    printf("  ");
    for (unsigned int column = 0; column < dimensions; column++)
    {
      double element;
      if (indexMatrix(&matrix, row, column, &element))
      {
        printf("%.2f\t", element);
      }
      else
      {
        printf("%.2f\t", 0.0);
      }
    }
    printf("\n");
  }
#endif

  return matrix;
}