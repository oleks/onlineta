#include <stdio.h>
#include <unistd.h>

// A simple program stalls until user gives input.

int main()
{
  printf("Press any key to stop stalling..");
  getchar();
  return 0;
}
