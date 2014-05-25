#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

WD=$(pwd)

teardown_cgroup()
{
  if [ -d "$WD/cgroup/$1/onlineta" ]; then
    rmdir "$WD/cgroup/$1/onlineta" || exit 1
  fi  

  if [ -d "$WD/cgroup/$1" ]; then
    umount "$WD/cgroup/$1" || exit 1
    rmdir "$WD/cgroup/$1" || exit 1
  fi  
}

teardown_cgroup "cpu,cpuacct"
teardown_cgroup "memory"

if [ -d "$WD/cgroup" ]; then
  umount "$WD/cgroup" || exit 1
  rmdir "$WD/cgroup" || exit 1
