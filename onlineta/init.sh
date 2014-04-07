#!/usr/bin/env sh

WD=$(pwd)

mkdir -p "$WD"/cgroup
MNT=$(mount | grep "$WD"/cgroup)
if [ -z "$MNT" ]; then
  mount -t tmpfs none cgroup
fi

mkdir -p cgroup/memory
MNT=$(mount | grep "$WD"/cgroup/memory)
if [ -z "$MNT" ]; then
  mount -t cgroup -o memory none cgroup/memory
fi

