#define _GNU_SOURCE
#include <sys/mount.h> // mount, umount
#include <stdio.h> // perror, FILE
#include <stdlib.h> // exit, EXIT_*
#include <mntent.h> // *mntent*
#include <errno.h> // errno, EBUSY
#include <string.h> // strncmp
#include <unistd.h> // execv
#include <sys/resource.h> // *rlimit*

static int
mount_proc()
{
  if (mount("proc", "/home", "proc", 0, NULL))
  {
    perror("mount proc under new root");
    exit(EXIT_FAILURE);
  }
  return 0;
}

static inline int
try_unmount_mnt_dir(char *mnt_dir)
{
  if(umount(mnt_dir) == -1 && errno != EBUSY)
  {
    // Something really bad happened.
    perror("unmount a dir");
    exit(EXIT_FAILURE);
  }
  return 0;
}

static int
try_unmount_oldroot()
{
  FILE *mntfile;
  struct mntent *mntent;
  int found;

  found = 0;
  mntfile = setmntent("/home/mounts", "r");
  while ((mntent = getmntent(mntfile)) != NULL)
  {
    if (strncmp(mntent->mnt_dir, "/.oldroot", 9) == 0)
    {
      found = 1;
      try_unmount_mnt_dir(mntent->mnt_dir);
    }
  }
  endmntent(mntfile);

  return found;
}

static int
unmount_oldroot()
{
  int depth = 20;
  while (try_unmount_oldroot() && (depth--)); // Oh my!
  // This is probably as fast as anything else in practice anyhow..

  if (depth == 0)
  {
    perror("couldn't unmount old root");
    exit(EXIT_FAILURE);
  }
  return 0;
}

static int
unmount_proc()
{
  if (umount("/home"))
  {
    perror("umount proc under new root");
    exit(EXIT_FAILURE);
  }
  return 0;
}

static int
limit_resources()
{
  struct rlimit rlimit;

  rlimit.rlim_cur = 20;
  rlimit.rlim_max = 20;
  if (setrlimit(RLIMIT_NPROC, &rlimit))
  {
    perror("limit nproc");
    exit(EXIT_FAILURE);
  }

  rlimit.rlim_cur = 0;
  rlimit.rlim_max = 0;
  if (setrlimit(RLIMIT_CORE, &rlimit))
  {
    perror("limit core size");
    exit(EXIT_FAILURE);
  }

  rlimit.rlim_cur = 1;
  rlimit.rlim_max = 1;
  if (setrlimit(RLIMIT_CPU, &rlimit))
  {
    perror("limit cpu time");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  argc = argc;

  mount_proc();
  unmount_oldroot();
  unmount_proc();

  setuid(1000);

  limit_resources();

  if (execv(argv[1], argv + 1))
  {
    perror("exec under new root");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
