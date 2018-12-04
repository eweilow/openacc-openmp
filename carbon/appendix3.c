struct SparseMatrix
{
  unsigned int dimensions;
  unsigned int elementCount;
  double *elements;
  unsigned int *elementColumns;
  unsigned int *rowOffsets;
};

void sparseMatrixMultiply(
  struct SparseMatrix *matrix,
  double *in,
  double *out
) {
  for (int row = 0; row < matrix->dimensions; ++row) {
    int startOffset = matrix->rowOffsets[row];
    int endOffset = matrix->rowOffsets[row + 1];
    for (int i = startOffset; i < endOffset; ++i) {
      int column = matrix->elementColumns[i];
      out[row] += matrix->elements[i] * in[column];
    }
  }
}