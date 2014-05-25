#define _GNU_SOURCE // necessary to get CLONE_*
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <stdlib.h> // exit, EXIT_*
#include <stdio.h> // perror
#include <unistd.h> // clone
#include <sched.h> // CLONE_*

#include <fcntl.h> // O_*
#include <semaphore.h> // sem_*

static sem_t *MAPPING;

static int
exec(void *arg)
{
  arg = arg;
//  char **argv = (char **)arg;
  sem_wait(MAPPING);
  sem_close(MAPPING);
  printf("%d\n", getuid());
  return 0;
  //execv(argv[0], argv);
  //perror("exec");
  //exit(EXIT_FAILURE);
}

#define STACK_SIZE (1024 * 1024)

static char
stack[STACK_SIZE];

static int
chdir_to_uid_map(pid_t pid)
{
  #define BUF_SIZE 32

  char pid_str[BUF_SIZE];

  if (chdir("/proc"))
  {
    perror("Couldn't navigate to /proc");
    exit(EXIT_FAILURE);
  }

  if (snprintf(pid_str, BUF_SIZE, "%d", pid) < 1)
  {
    perror("pid to string");
    exit(EXIT_FAILURE);
  }

  if (chdir(pid_str))
  {
    perror("Couldn't navigate to /proc/[pid]");
    exit(EXIT_FAILURE);
  }

  return 0;

  #undef BUF_SIZE
}

static int
uid_map()
{
  FILE *uid_map_file;

  uid_map_file = fopen("uid_map", "ae"); // O_APPEND and O_CLOEXEC

  if (!uid_map_file)
  {
    perror("Couldn't open uid_map file");
    exit(EXIT_FAILURE);
  }

  if (fprintf(uid_map_file, "0 %d 1", getuid()) < 1)
  {
    perror("Couldn't map the contained user to root");
    exit(EXIT_FAILURE);
  }
 
  if (fclose(uid_map_file))
  {
    perror("Couldn't close the uid_map file");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int
main(int argc, char  *argv[])
{
  pid_t pid;

  argc = argc;

  MAPPING = sem_open("/MAPPING", O_CREAT, 0644, 0);
  if (MAPPING == SEM_FAILED)
  {
    perror("Open process-shared semaphore for userns");
    exit(EXIT_FAILURE);
  }

  pid = clone(exec, stack + STACK_SIZE, CLONE_NEWUSER | SIGCHLD, argv + 1);

  if (pid == -1)
  {
    perror("new userns");
    exit(EXIT_FAILURE);
  }

  chdir_to_uid_map(pid);
  uid_map();
  sem_post(MAPPING);
  sem_close(MAPPING);

  if (waitpid(pid, NULL, 0) == -1)
  {
    perror("wait for userns");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
