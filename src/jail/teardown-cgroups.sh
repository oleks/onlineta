#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

CGROUPS_DIR=cgroups-target

teardown_cgroup()
{
  if [ -d "$CGROUPS_DIR/$1/onlineta" ]; then
    rmdir "$CGROUPS_DIR/$1/onlineta" || exit 1
  fi

  if [ -d "$CGROUPS_DIR/$1" ]; then
    umount "$CGROUPS_DIR/$1" || exit 1
    rmdir "$CGROUPS_DIR/$1" || exit 1
  fi
}

teardown_cgroup "devices"
teardown_cgroup "cpu,cpuacct"
teardown_cgroup "memory"

if [ -d "$CGROUPS_DIR" ]; then
  umount "$CGROUPS_DIR" || exit 1
  rmdir "$CGROUPS_DIR" || exit 1
fi
