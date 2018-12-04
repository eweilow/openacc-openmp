#include <stdlib.h>
#include <stdbool.h>

// generate N random values into randomValues
void generateRandomValues(unsigned int *randomValues, unsigned int N)
{
  for (unsigned int i = 0; i < N; i++)
  {
    randomValues[i] = rand();
  }
}

// generate N random values into randomValues
void generateRandomDoubles(double *randomValues, unsigned int N, double a, double b)
{
  for (unsigned int i = 0; i < N; i++)
  {
    randomValues[i] = (rand() / (double)RAND_MAX) * (b - a) + a;
  }
}

// Pick n distinct random integers on the interval [0, maxSize)
void pickRandom(unsigned int *into, unsigned int count, unsigned int maxSize)
{
  unsigned long bytes = sizeof(bool) * maxSize;
  bool *isPicked = (bool *)malloc(bytes);
  for (int n = 0; n < maxSize; n++)
  {
    isPicked[n] = false;
  }

  unsigned int next = rand() % maxSize;

  unsigned int picked = 0;
  while (picked < count)
  {
    if (!isPicked[next] && !(next > 9 && next < 25))
    {
      isPicked[next] = true;
      ++picked;
    }
    next = (next + rand()) % maxSize;
  }

  unsigned int head = 0;
  for (unsigned int n = 0; n < maxSize; n++)
  {
    if (isPicked[n])
    {
      into[head++] = n;
    }
  }

  free(isPicked);
}

/*
  unsigned int *into = (unsigned int *)malloc(sizeof(unsigned int) * 10);
  pickRandom(into, 10, 1000);

  for (int n = 0; n < 10; n++)
  {
    printf("%d\n", into[n]);
  }
*/