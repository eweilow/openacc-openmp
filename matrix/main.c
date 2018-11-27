#include <time.h>
#include <openacc.h>

#define LENGTH 100000
#define BATCH_ROWS LENGTH
#define BATCH_COLUMNS 5000

//int a[LENGTH][LENGTH];
float b[LENGTH];
float c[LENGTH];

int main()
{
  printf("Starting matrix multiplication\n");

  for (int i = 0; i < LENGTH; i++)
  {
    for (int j = 0; j < LENGTH; j++)
    {
      //a[i][j] = i == j ? 1 : 0;
    }
  }

  for (int i = 0; i < LENGTH; i++)
  {
    b[i] = (float)i;
    c[i] = 0.0;
  }

  clock_t begin = clock();

  // 00 + 11 + 22 + ... + nn
  // (00 + 11 + 22 + ... + kk) + () + ... + () + (mm + .. + nn)
  /*
  c0 = a00 b0 + a10 b1
  c1 = a01 b0 + a11 b1
*/
  for (int k = 0; k < LENGTH / BATCH_COLUMNS; k++)
  {
    int fromJ = k * BATCH_COLUMNS;
    int toJ = k * BATCH_COLUMNS + BATCH_COLUMNS;
    printf("%d / %d\n", k, LENGTH / BATCH_COLUMNS);
    //printf("%d / %d\n %d ... %d (%d columns)\n", k, LENGTH / BATCH_COLUMNS, fromJ, toJ, toJ - fromJ);
    for (int l = 0; l < LENGTH / BATCH_ROWS; l++)
    {
      int fromI = l * BATCH_ROWS;
      int toI = l * BATCH_ROWS + BATCH_ROWS;

//#pragma acc loop copyin(b [fromJ:toJ]) create(c [fromI:toI]) copyout(c [fromI:toI]) private(sum)
#pragma acc kernels copyin(b) copyout(c)
      {
#pragma acc loop worker
        for (int i = fromI; i < toI; ++i)
        {
          float sum = 0.0;
#pragma acc loop vector reduction(+ \
                                  : sum)
          for (int j = fromJ; j < toJ; j++)
          {
            sum += (i == j ? 1.0 : 0.0) * b[j];
          }

#pragma acc atomic
          c[i] += sum;
        }
      }
    }
  }

  clock_t end = clock();
  printf("Computation took %f ms\n", 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC);

  for (int i = 0; i < 10; i++)
  {
    printf("%d ", b[i]);
  }
  printf("\n");
  for (int i = 0; i < 10; i++)
  {
    printf("%d ", c[i]);
  }
  printf("\n");
  return 0;
}