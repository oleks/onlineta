#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

mount -t tmpfs -o size=1M tmpfs "tmpfs-target" || exit 1
cp -r "input/"* "tmpfs-target/" || exit 1

./mnt ./rootfs ./minpuf ./pivot-root /init /home/student/a.out

rm -rf "output/"*
cp -r "tmpfs-target/"* "output/"
umount "tmpfs-target"
