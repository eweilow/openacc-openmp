#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MATRIX_SIZE 1000000
#define SPARSENESS_PERCENTAGE 0.01
#define SPARSENESS (float)(SPARSENESS_PERCENTAGE / 100.0)

const int RANDOM_STEP_FACTOR = (int)(1.0 / SPARSENESS);

#define random(rndval) ((float)(rndval) / (float)RAND_MAX)
#define randomStep(rndval) (((rndval)*2 * RANDOM_STEP_FACTOR) / RAND_MAX)
#define element(rndval) ((random(rndval) - 0.5) * 2.0)

int main()
{
  // Initialize RNG
  time_t t;
  srand((unsigned)time(&t));

  float el;

  unsigned long expectedCountPerRow = (unsigned long)(MATRIX_SIZE * SPARSENESS);
  unsigned long allocationPerRow = (unsigned long)pow(2, ceil(log2((double)expectedCountPerRow)));
  printf("Expecting %lu elements\n", expectedCountPerRow * (unsigned long)MATRIX_SIZE);

  printf("Expecting %d entries per row\n", expectedCountPerRow);
  printf("Allocating %d entries per row\n", allocationPerRow);

  float *row = (float *)malloc(sizeof(float) * allocationPerRow);
  int *steps = (int *)malloc(sizeof(int) * allocationPerRow);
  int *rndvals = (int *)malloc(sizeof(int) * allocationPerRow);

  if (row == NULL)
  {
    printf("Allocation failed.");
    return 1;
  }

  printf("Computing matrix\n");
  unsigned long count = 0;
  for (unsigned long i = 0; i < MATRIX_SIZE; i++)
  {
    int index = 1;
    //printf("%d %d %.4f %.4f\n", i, randomStep(), random(), random() * (float)RANDOM_STEP_FACTOR);
    for (unsigned long l = 0; l < expectedCountPerRow; l++)
    {
      rndvals[l] = rand();
    }
    for (unsigned long l = 0; l < expectedCountPerRow; l++)
    {
      steps[l] = randomStep(rndvals[l]);
    }
    for (unsigned long j = i + steps[0]; j < MATRIX_SIZE; j += steps[index])
    {
      row[index] = element(rndvals[index]);

      if (++index > expectedCountPerRow)
      {
        continue;
      }
    }
    count += index;
  }

  printf("Sparseness: %f%%", (double)(2.0 * count) / ((double)MATRIX_SIZE * (double)MATRIX_SIZE / 100.0));

  return 0;
}