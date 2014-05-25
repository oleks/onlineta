#include <stdio.h> // perror, FILE
#include <stdlib.h> // exit, EXIT_*
#include <unistd.h> // setuid

int
main(int argc, char *argv[])
{
  argc = argc;

  if (setuid(1000))
  {
    perror("setuid");
    exit(EXIT_FAILURE);
  }

  execv(argv[1], argv + 1);
  perror("execv after setuid");
  exit(EXIT_FAILURE);
}
