#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

teardown_cgroup()
{
  if [ -d "cgroup/$1/onlineta" ]; then
    rmdir "cgroup/$1/onlineta" || exit 1
  fi

  if [ -d "cgroup/$1" ]; then
    umount "cgroup/$1" || exit 1
    rmdir "cgroup/$1" || exit 1
  fi
}

teardown_cgroup "cpu,cpuacct"
teardown_cgroup "memory"

if [ -d "cgroup" ]; then
  umount "cgroup" || exit 1
  rmdir "cgroup" || exit 1
fi
