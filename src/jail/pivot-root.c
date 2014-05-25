#define _GNU_SOURCE
#include <stdio.h> // perror
#include <stdlib.h> // exit, EXIT_*
#include <syscall.h> // SYS_pivot_root
#include <unistd.h> // syscall, chdir, execv

int
main(int argc, char *argv[])
{
  argc = argc;

  if (syscall(SYS_pivot_root, "rootfs-target", "rootfs-target/.oldroot"))
  {
    perror("pivot root");
    exit(EXIT_FAILURE);
  }

  chdir("/");

  if (execv(argv[1], argv + 1))
  {
    perror("exec under new root");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
