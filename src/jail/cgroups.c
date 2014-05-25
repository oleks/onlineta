#define _GNU_SOURCE // necessary to get CLONE_*
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <stdlib.h> // exit, EXIT_*
#include <stdio.h> // perror
#include <unistd.h> // clone
#include <sched.h> // CLONE_*

static const char USAGE[] = "Usage: cgroups [-g <path>]";
static pid_t PID;

static int
attach_to_cgroup(char *tasks_file_path)
{
  FILE *tasks_file;

  tasks_file = fopen(tasks_file_path, "ae"); // O_APPEND and O_CLOEXEC
  if (!tasks_file)
  {
    perror("Couldn't open cgroup tasks file");
    exit(EXIT_FAILURE);
  }

  if (fprintf(tasks_file, "%d", PID) < 1)
  {
    perror("Couldn't attach task to cgroup");
    exit(EXIT_FAILURE);
  }

  if (fclose(tasks_file))
  {
    perror("Couldn't close cgroups tasks file");
    exit(EXIT_FAILURE);
  }

  return 0;
}

static int
attach_to_cgroups(int argc, char *argv[])
{
  int opt;
  opterr = 0; // prevent writing to stderr on invalid character.
  while ((opt = getopt(argc, argv, "+g:")) != -1)
  // Stop option processing as soon as a nonoption is encountered.
  // Demand an option argument for the -g option.
  {
    switch (opt)
    {
      case 'g':
        if (attach_to_cgroup(optarg))
        {
          printf("Couldn't attach to cgroup %s\n", optarg);
          exit(EXIT_FAILURE);
        }
        break;
      default:
        printf(USAGE);
        exit(EXIT_FAILURE);
        break;
    }
  }
  return optind;
}

int
main(int argc, char  *argv[])
{
  int argi;

  argi = attach_to_cgroups(argc, argv);

  if (argi < argc)
  {
    execv(argv[argi], argv + argi);
    perror("exec within control group");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
