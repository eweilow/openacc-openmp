#define MATRIX_SIZE 700000

void initializeVectors(float a[MATRIX_SIZE], float b[MATRIX_SIZE])
{
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    a[i] = (float)i;
    b[i] = 0.0;
  }
}

float matrixElement(int row, int column)
{
  if (row == column)
  {
    return 1.0;
  }
  return 0.0;
}

void printVector(float vec[MATRIX_SIZE])
{
  const int standardSize = 10;
  int size = MATRIX_SIZE < standardSize ? MATRIX_SIZE : standardSize;
  for (int i = 0; i < size; i++)
  {
    printf("%.2f ", vec[i]);
  }
  printf("\n");
}

void compareVectors(float a[MATRIX_SIZE], float b[MATRIX_SIZE])
{
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    if (fabs(a[i] - b[i]) > 0.001)
    {
      printf("mismatch at index %d: %f != %f\n", i, a[i], b[i]);
    }
  }
  printf("\n");
}