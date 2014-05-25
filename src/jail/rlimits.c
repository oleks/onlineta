#include <stdio.h> // perror, FILE
#include <stdlib.h> // exit, EXIT_*
#include <mntent.h> // *mntent*
#include <string.h> // strncmp
#include <unistd.h> // execv
#include <sys/resource.h> // *rlimit*

static int
limit_resources()
{
  struct rlimit rlimit;

  rlimit.rlim_cur = 20;
  rlimit.rlim_max = 20;
  if (setrlimit(RLIMIT_NPROC, &rlimit))
  {
    perror("RLIMIT_NPROC");
    exit(EXIT_FAILURE);
  }

  rlimit.rlim_cur = 0;
  rlimit.rlim_max = 0;
  if (setrlimit(RLIMIT_CORE, &rlimit))
  {
    perror("RLIMIT_CORE");
    exit(EXIT_FAILURE);
  }

  rlimit.rlim_cur = 1;
  rlimit.rlim_max = 1;
  if (setrlimit(RLIMIT_CPU, &rlimit))
  {
    perror("RLIMIT_CPU");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  argc = argc;

  limit_resources();

  execv(argv[1], argv + 1);
  perror("execv after rlimits");
  exit(EXIT_FAILURE);
}
