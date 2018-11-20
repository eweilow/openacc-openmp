#include <stdio.h>
#include <time.h>


struct Result {
  long operationsRun;
  double millisecondsSpent;
  double microsecondsPerOperation;
};

  /* Here lies the actual code */
void executeSummation(int range) {
  int output[range][range];

  #pragma acc kernels
  {
    #pragma omp parallel for
    for(int a = 0; a < range; a++) {
      for(int b = 0; b < range; b++) {
        int result = 0;
        for(int n = 0; n < (a + b); n++) {
          result += n;
        }
        output[a][b] += result;
      }
    }
  }

}

struct Result runComputation(int range) {
  struct Result result;
  clock_t begin = clock();

  executeSummation(range);

  clock_t end = clock();
  result.millisecondsSpent = 1000.0 * (double)(end - begin) / CLOCKS_PER_SEC;
  result.microsecondsPerOperation = 1000.0 * result.millisecondsSpent / (double)(range * range);
  return result;
}

int main()
{
  struct Result result = runComputation(10000);

  printf("\nRan %d ops\n", result.operationsRun); 
  printf("Total execution time: %f ms\n", result.millisecondsSpent);
  printf("Total time per op: %f microsecs\n", result.microsecondsPerOperation);

  return 0;
}