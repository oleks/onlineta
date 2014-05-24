#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
  int32_t max;
  int32_t val;
  while (scanf("%d\n", &val))
  {
    if (val > max)
    {
      max = val;
    }
  }
  printf("%d\n", max);
}
