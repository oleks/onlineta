// A lightweight program to start a program in a control group.
// Usage: libexec <path-to-tasks-file> <executable> <arguments>.

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  pid_t pid;
  FILE *fd;

  if (argc < 3)
  {
    return 1;
  }

  pid = getpid();
  fd = fopen(argv[1], "ae");
  if (!fd)
  {
    return 2;
  }
  if (fprintf(fd, "%d\n", pid) < 0)
  {
    return 2;
  }
  if (fclose(fd) != 0)
  {
    return 2;
  }

  execve(argv[2], argv + 3, NULL);
}
