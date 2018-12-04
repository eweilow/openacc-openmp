void matrixMultiply(
    struct SparseMatrix *matrix,
    double *inputVector,
    double *outputVector)
{
  for (int row = 0; row < matrix->dimensions; ++row)
  {
    int startOffset = matrix->rowOffsets[row];
    int endOffset = matrix->rowOffsets[row + 1];

    float sum = 0.0;
    for (int i = startOffset; i < endOffset; ++i)
    {
      float matrixElement = matrix->elements[i];
      int column = matrix->elementColumns[i];

      sum += matrixElement * inputVector[column];
    }
    outputVector[row] = sum;
  }
}

//#define VEC_DEBUG

void matrixIterate(
    struct SparseMatrix *matrix,
    double *inputVector,
    double *outputVector,
    unsigned int iterations)
{
  double *input = inputVector;
  double *output = outputVector;
  for (unsigned int n = 0; n < iterations; n++)
  {
    matrixMultiply(matrix, input, output);

#ifdef VEC_DEBUG
    printf(" Multiplied %d times...\n", n + 1);

    printf(" in:\n");
    printVector(input, matrix->dimensions);

    printf(" out:\n");
    printVector(output, matrix->dimensions);
#endif

    double *tmp = input;
    input = output;
    output = tmp;
  }
}