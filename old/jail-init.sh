#!/usr/bin/env bash

# this is bad as bash is using config files in root and home

mount --make-rprivate /
#mount --rbind root readonly
#mount -o ro --make-rslave readonly
mount -o ro -t squashfs --make-rslave squash readonly
mount --bind output readonly/output
#mount
pivot_root readonly readonly/root/oldroot
PATH=/bin
#umount /root/oldroot/run
mount -t proc proc /proc
lsof
losetup -a
ps aux
/init
mount
