#define _GNU_SOURCE // necessary to get CLONE_*
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <stdlib.h> // exit, EXIT_*
#include <stdio.h> // perror
#include <unistd.h> // clone
#include <sched.h> // CLONE_*

static int
exec(void *arg)
{
  char **argv = (char **)arg;
  execv(argv[0], argv);
  perror("execv");
  exit(EXIT_FAILURE);
}

#define STACK_SIZE (1024 * 1024)

static char
stack[STACK_SIZE];

int
main(int argc, char  *argv[])
{
  pid_t pid;

  argc = argc;

  pid = clone(exec, stack + STACK_SIZE,
    CLONE_NEWNS | CLONE_NEWIPC | CLONE_NEWNET | // min
    CLONE_NEWPID | CLONE_NEWUTS | CLONE_IO | // puf
    SIGCHLD, argv + 1);

  if (pid == -1)
  {
    perror("clone");
    exit(EXIT_FAILURE);
  }

  if (waitpid(pid, NULL, 0) == -1)
  {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
