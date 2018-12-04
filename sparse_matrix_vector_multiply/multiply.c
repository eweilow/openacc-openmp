
#define BATCH 200000

void matrixMultiply(
    struct SparseMatrix *matrix,
    double *inputVector,
    double *outputVector)
{
  for (int batch = 0; batch < matrix->dimensions / BATCH; ++batch)
  {
    int from = batch * BATCH;
    int to = min(matrix->dimensions, batch * BATCH + BATCH);
#pragma acc parallel loop gang worker copyin(from, to) present(matrix, matrix->dimensions, matrix->rowOffsets, matrix->elements, matrix->elementColumns, inputVector, outputVector)
    for (int row = from; row < to; ++row)
    {
      int startOffset = matrix->rowOffsets[row];
      int endOffset = matrix->rowOffsets[row + 1];

      float sum = 0.0;
#pragma acc loop vector reduction(+ \
                                  : sum)
      for (int i = startOffset; i < endOffset; ++i)
      {
        float matrixElement = matrix->elements[i];
        int column = matrix->elementColumns[i];

        sum += matrixElement * inputVector[column];
      }
      outputVector[row] = sum;
    }
  }
}

//#define VEC_DEBUG

void matrixIterate(
    struct SparseMatrix *matrix,
    double *inputVector,
    double *outputVector,
    unsigned int iterations)
{
  if (matrix->elementCount == 0)
  {
    return;
  }

  double *input = inputVector;
  double *output = outputVector;
#pragma acc data copyin(matrix [0:1],                                    \
                        matrix->elements [0:matrix->elementCount],       \
                        matrix->elementColumns [0:matrix->elementCount], \
                        matrix->rowOffsets [0:matrix->dimensions + 1],   \
                        inputVector [0:matrix->dimensions],              \
                        outputVector [0:matrix->dimensions])             \
    copyout(outputVector [0:matrix->dimensions])
  for (unsigned int n = 0; n < iterations; n++)
  {
#ifdef VEC_DEBUG
    printf(" Multiplying...\n", n);
#endif
    matrixMultiply(matrix, input, output);
#ifdef VEC_DEBUG
    printf(" Multiplied %d times...\n", n + 1);
#endif
#ifdef VEC_DEBUG
#pragma acc update host(inputVector [0:matrix->dimensions], outputVector [0:matrix->dimensions])
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