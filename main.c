#include <stdio.h>
int main()
{
  const int range = 10;
  int output[range][range];

  for(int a = 0; a < range; a++) {
    for(int b = 0; b < range; b++) {
      output[a][b] = a + b;
    }
  }

  for(int a = 0; a < range; a++) {
    for(int b = 0; b < range; b++) {
      printf("%d + %d = %d\n", a, b, output[a][b]);
    }
  }

  return 0;
}