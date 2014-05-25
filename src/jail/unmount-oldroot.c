#define _GNU_SOURCE
#include <sys/mount.h> // mount, umount
#include <stdio.h> // perror, FILE
#include <stdlib.h> // exit, EXIT_*
#include <mntent.h> // *mntent*
#include <errno.h> // errno, EBUSY
#include <string.h> // strncmp
#include <unistd.h> // execv

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

int
main(int argc, char *argv[])
{
  argc = argc;

  mount_proc();
  unmount_oldroot();
  unmount_proc();

  execv(argv[1], argv + 1);
  perror("execv after unmount-oldroot");
  exit(EXIT_FAILURE);
}
