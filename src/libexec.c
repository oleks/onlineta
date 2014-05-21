// NON-WORKING

// A lightweight program to start a program in a control group.
// Usage: libexec <path-to-tasks-file> <executable> <arguments>.

#define _GNU_SOURCE // necessary for unshare(2)
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>

#include <errno.h>
#include <string.h>

static
pid_t PID;

int
cat_pid_to_file(char *path)
{
  FILE *fd;


  fd = fopen(path, "ae");
  if (!fd)
  {
    exit(EXIT_FAILURE);
  }
  if (fprintf(fd, "%d", PID) < 0)
  {
    exit(EXIT_FAILURE);
  }
  if (fclose(fd) != 0)
  {
    exit(EXIT_FAILURE);
  }

  return 0;
}

int
strtoi(char *arg)
{
  long int long_value;

  long_value = strtol(arg, NULL, 10);
  if (long_value < 0 || long_value > INT_MAX)
  {
    exit(EXIT_FAILURE);
  }
  return (int)long_value;
}

int set_time_limit(int seconds)
{
  struct rlimit limit_cpu;

  limit_cpu.rlim_cur = seconds;
  limit_cpu.rlim_max = seconds;

  if (setrlimit(RLIMIT_CPU, &limit_cpu) != 0)
  {
    exit(EXIT_FAILURE);
  }

  return 0;
}

int
process_args(int argc, char *argv[])
{
  int opt;
  opterr = 0; // prevent writing to stderr on invalid character.
  while ((opt = getopt(argc, argv, "+:g:t:")) != -1)
  {
    switch (opt)
    {
    case 'g':
      if (cat_pid_to_file(optarg) != 0)
      {
        exit(EXIT_FAILURE);
      }
      break;
    case 't':
      if(set_time_limit(strtoi(optarg)) != 0)
      {
        exit(EXIT_FAILURE);
      }
      break;
    case '?':
      printf(
        "Usage: [-g /path/to/cgroup/tasks] [-t nsecs] program arguments\n");
      exit(EXIT_FAILURE);
      break;
    }
  }
  return optind;
}

static int
runinit(void *argv)
{
  printf("YAY!\n");
  if (mount("none", "/", "none", MS_REC | MS_PRIVATE, NULL)
      // recursively mark / as private)
     || mount("root", "readonly", "bind",
        MS_BIND | MS_REC | MS_RDONLY | MS_SLAVE, NULL))
//     || mount("squash", "readonly", "squashfs",
//        0, NULL))
  {
    printf("failed mount %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("mount ok?\n");

  syscall(SYS_pivot_root, "readonly", "readonly/root/oldroot");

  printf("still ok?\n"); // 04500

  chdir("/");

  printf("okay..\n");

  char **av;
  av = malloc(2 * sizeof(av[0]));
  av[0] = "/init";
  av[1] = NULL;
/*
  FILE *fd;
  fd = fopen("/etc/ld.so.cache", "r");
  if (fd == NULL)
  {
    printf("FAIL %s\n", strerror(errno));
  }

  if (mount("/lib", "/lib", "none", MS_BIND | MS_RDONLY, NULL))
  {
    printf("MEH %s\n", strerror(errno));
  }
*/
  if (execvp("/init", av) == -1)
  {
    printf("failed exec %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("darn\n");
  return 0;
}

int
main(int argc, char* argv[])
{
  int argi;
  pid_t child;
  int child_status;

  int flags = (CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWNS
         | CLONE_NEWUTS | CLONE_NEWPID | SIGCHLD);

  char *stack = malloc (0x10000);
  stack += 0x10000;

  PID = getpid();
  argi = process_args(argc, argv);

  if (argi >= argc)
  {
    exit(EXIT_FAILURE);
  }

  // unshare -minpuf --mount-proc bash # -m is implied
/*
  if (unshare(CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWNS | CLONE_NEWPID | \
    CLONE_NEWUTS) != 0)
  {
    printf("UNSHARE FAILED: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (seteuid(1000) != 0)
  {
    printf("UNSHARE FAILED: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("PID %d\n", getpid());
*/
//  sethostname("test", 4);

  child = clone(runinit, stack, flags, NULL);

  while(waitpid(-1, &child_status, __WALL) != child)
    ;

  if (WIFEXITED(child_status))
  {
    printf("EXITED WITH %d\n", WEXITSTATUS(child_status));
  }

  exit(EXIT_SUCCESS);
}
