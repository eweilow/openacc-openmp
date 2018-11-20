#include <stdio.h>
#include <time.h>
int main()
{
  clock_t begin = clock();

  const int range = 10000;
  const int totalOperations = range * range;
  int output[range][range];
  
  #pragma omp parallel
  #pragma acc kernels
  {
    for(int a = 0; a < range; a++) {
      for(int b = 0; b < range; b++) {
        int result = 0;
        for(int n = 0; n < a + b; n++) {
          result += n;
        }
        output[a][b] = result;
      }
    }
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  double time_per_op = time_spent / (double)(totalOperations);

  int a = 1000;
  int b = 1313;
  printf("\nRan %d ops\n", totalOperations);
  printf("Total execution time: %f ms\n", time_spent * 1000.0);
  printf("Total time per op: %f microsecs\n", time_per_op * 1000000.0);
  printf("Sample result: %d + %d = %d\n", a, b, output[a][b]);

  return 0;
}