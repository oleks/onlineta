#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid;

  while ((pid = fork()) != -1)
  {
    if (pid != 0)
    {
      printf(".");
    }
  }
  return 0;
}
