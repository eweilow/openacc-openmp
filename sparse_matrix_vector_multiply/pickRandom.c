#include <stdlib.h>
#include <stdbool.h>

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
    if (!isPicked[next])
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