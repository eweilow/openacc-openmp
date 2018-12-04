#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "./pickRandom.c"

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
    unsigned int expectedCountPerRow,
    unsigned int expectedElements)
{
  printf(" matrix info:\n");
  printf(" - dimensions: 10^%d x 10^%d (%d x %d)\n", (unsigned int)log10(dimensions), (unsigned int)log10(dimensions), dimensions, dimensions);
  printf(" - sparseness: %.2f%%\n", sparseness * 100.0);

  printf(" memory info:\n");
  printf(" - 2^%d (%d) elements per row\n", (int)ceil(log2((double)expectedCountPerRow)), expectedCountPerRow);
  printf(" - 2^%d (%d) elements in total\n", (int)ceil(log2((double)expectedElements)), expectedElements);
  printf(" - %.2f MB necessary\n", (double)bytes / 1e6);
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

// #define DEBUG

// Generate a sparse matrix
struct SparseMatrix generateMatrix(unsigned int dimensions, double sparseness)
{
  struct SparseMatrix matrix;
  matrix.dimensions = dimensions;

  // The expected amount of elements per row, based on sparseness
  unsigned int expectedCountPerRow = (unsigned int)((double)dimensions * (1.0 - sparseness));
  // The expected amount of elements necessary to allocate
  unsigned int expectedElements = (unsigned int)((1.0 - sparseness) * (double)dimensions * dimensions);

  // Just used to print information
  unsigned int bytes = expectedElements * (sizeof(double) + sizeof(unsigned int)) + // From matrix.elements
                       (dimensions + 1) * sizeof(unsigned int) +                    // From matrix.rowOffsets
                       expectedElements * (sizeof(unsigned int));                   // From matrix.elementColumns
  printMatrixInfo(
      dimensions,
      sparseness,
      bytes,
      expectedCountPerRow,
      expectedElements);

  matrix.elements = (double *)malloc(sizeof(double) * expectedElements);
  matrix.elementColumns = (unsigned int *)malloc(sizeof(unsigned int) * expectedElements);
  matrix.rowOffsets = (unsigned int *)malloc(sizeof(unsigned int) * (dimensions + 1));
  matrix.elementCount = 0;

  // indicesWithNonZero is a sorted vector with distinct integers on the range [0, expectedElements)
  unsigned int *indicesWithNonZero = (unsigned int *)malloc(sizeof(unsigned int) * expectedElements);
  pickRandom(indicesWithNonZero, expectedElements, dimensions * dimensions);

#ifdef DEBUG
  printf(" generation:\n");
#endif

  unsigned int elementHead = 0; // The position in matrix.elements, matrix.elementColumns
  unsigned int rowHead = 0;     // The current row
  for (unsigned int n = 0; n < expectedElements; n++)
  {
    unsigned int row = indicesWithNonZero[n] / dimensions;
    unsigned int column = indicesWithNonZero[n] % dimensions;

    while (row > rowHead)
    {
#ifdef DEBUG
      printf("  increment row\n");
#endif
      matrix.rowOffsets[++rowHead] = elementHead;
    }

#ifdef DEBUG
    printf("   %d (row %d, col %d) heads: (row: %d / %d), (elements: %d / %d)\n", indicesWithNonZero[n], row, column, rowHead, dimensions, elementHead, expectedElements - 1);
#endif
    matrix.elements[elementHead] = (double)indicesWithNonZero[n];
    matrix.elementColumns[elementHead] = column;
    ++elementHead;
  }
  matrix.rowOffsets[0] = 0;
  matrix.rowOffsets[rowHead + 1] = elementHead;
  matrix.elementCount = elementHead;

  generateRandomDoubles(matrix.elements, matrix.elementCount, -1.0, 1.0);

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

  free(indicesWithNonZero);

  return matrix;
}