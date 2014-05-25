#define _GNU_SOURCE
#include <sys/mount.h> // MS_*
#include <sys/wait.h> // waitpid
#include <stdio.h> // perror
#include <stdlib.h> // exit, EXIT_*
#include <unistd.h> // mount

static int
setup_rootfs()
{
  if (mount("none", "/", "none",
    MS_PRIVATE | MS_REC, NULL))
  {
    perror("mount / rprivate");
    exit(EXIT_FAILURE);
  }

  if (mount("root", "rootfs-target", "bind",
    MS_BIND | MS_SLAVE | MS_RDONLY, NULL))
  {
    perror("mount rootfs-target");
    exit(EXIT_FAILURE);
  }

  if (mount("tmpfs-target", "rootfs-target/home/student", "bind",
    MS_BIND, NULL))
  {
    perror("mount tmpfs-target");
    exit(EXIT_FAILURE);
  }
  return 0;
}

static int
teardown_rootfs()
{
  if (umount("rootfs-target/home/student"))
  {
    perror("umount rootfs-target/home/student");
    exit(EXIT_FAILURE);
  }

  if (umount("rootfs-target"))
  {
    perror("umount rootfs-target");
    exit(EXIT_FAILURE);
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  pid_t pid;

  argc = argc;

  setup_rootfs();

  pid = fork();
  if (pid == 0)
  {
    execv(argv[1], argv + 1);
    perror("execv");
    exit(EXIT_FAILURE);
  } else if (waitpid(pid, NULL, 0) == -1) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }
  
  teardown_rootfs();
  exit(EXIT_SUCCESS);
}
