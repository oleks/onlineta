#!/usr/bin/env sh

WD=$(pwd)

mkdir -p "$WD"/cgroup
MNT=$(mount | grep "$WD"/cgroup)
if [ -z "$MNT" ]; then
  mount -t tmpfs tmpfs cgroup
fi

mkdir -p cgroup/memory
MNT=$(mount | grep "$WD"/cgroup/memory)
if [ -z "$MNT" ]; then
  mount -t cgroup -o memory cgroup cgroup/memory
fi

mkdir -p cgroup/memory/onlineta
chown -R oleks:wheel cgroup/memory/onlineta

mkdir -p cgroup/cpu
MNT=$(mount | grep "$WD"/cgroup/cpu)
if [ -z "$MNT" ]; then
  mount -t cgroup -o cpuacct,cpu cgroup cgroup/cpu
fi

mkdir -p cgroup/cpu/onlineta
chown -R oleks:wheel cgroup/cpu/onlineta

