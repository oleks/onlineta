CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu11
LDFLAGS=

all:
	mkdir -p "rootfs/home/student"
	mkdir -p "rootfs/.bin"
	mkdir -p "rootfs/.oldroot"
	rm -f squashfs
	mksquashfs rootfs squashfs
	mkdir -p "rootfs-target"
	mkdir -p "tmpfs-target"
	mkdir -p "input"
	mkdir -p "output"
	$(CC) $(CFLAGS) $(LDFLAGS) -o input/stall stall.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o input/forkbomb forkbomb.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o cgroups cgroups.c
	$(CC) $(CFLAGS) $(LDFLAGS) -pthread -o user user.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o mnt mnt.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o pumin pumin.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o mount-rootfs mount-rootfs.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o pivot-root pivot-root.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o umount-oldroot umount-oldroot.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o setuid setuid.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o rlimits rlimits.c
	cp umount-oldroot "rootfs/.bin/umount-oldroot"
	cp setuid "rootfs/.bin/setuid"
	cp rlimits "rootfs/.bin/rlimits"
