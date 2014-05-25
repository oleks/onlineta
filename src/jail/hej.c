#include <stdio.h>
#include <unistd.h>

int main()
{
  int x;
  printf("HEJ %d!\n", getuid());
  scanf("%d\n", &x);
}
