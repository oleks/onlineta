#!/usr/bin/env bash

# Demonstrating how to create a container with a scratchspace accessible from
# the host, such that the host can save the data left behind by the container
# once it finishes. See also ./exec.sh.

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
