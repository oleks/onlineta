#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

WD=$(pwd)
CGROUPS_DIR=cgroups-target

mkdir -p "$CGROUPS_DIR" || exit 1
MNT=$(mount | grep "$WD/$CGROUPS_DIR")
if [ -z "$MNT" ]; then
  mount -t tmpfs tmpfs "$CGROUPS_DIR" || exit 1
fi

setup_cgroup()
{
  mkdir -p "cgroups-target/$1" || exit 1
  MNT=$(mount | grep "$WD/$CGROUPS_DIR/$1")
  if [ -z "$MNT" ]; then
    mount -t cgroup -o "$1" cgroup "$CGROUPS_DIR/$1" || exit 1
  fi

  mkdir -p "$CGROUPS_DIR/$1/onlineta" || exit 1
  chown -R onlineta:onlineta "$CGROUPS_DIR/$1/onlineta" || exit 1
}

setup_cgroup "memory"
setup_cgroup "cpu,cpuacct"
setup_cgroup "devices"
