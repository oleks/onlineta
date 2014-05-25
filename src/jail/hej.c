#include <stdio.h>
#include <unistd.h>

int main()
{
  printf("HEJ %d!\n", getuid());
}
