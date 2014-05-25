 #!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

WD=$(pwd)

mkdir -p "$WD/cgroup" || exit 1
MNT=$(mount | grep "$WD/cgroup")
if [ -z "$MNT" ]; then
  mount -t tmpfs tmpfs "$WD/cgroup" || exit 1
fi

setup_cgroup()
{
  mkdir -p "$WD/cgroup/$1" || exit 1
  MNT=$(mount | grep "$WD/cgroup/$1")
  if [ -z "$MNT" ]; then
    mount -t cgroup -o "$1" cgroup "$WD/cgroup/$1" || exit 1
  fi

  mkdir -p "cgroup/$1/onlineta" || exit 1
  chown -R onlineta:onlineta "$WD/cgroup/$1/onlineta" || exit 1
}

setup_cgroup "memory"
setup_cgroup "cpu,cpuacct"
