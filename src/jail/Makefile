CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu11
LDFLAGS=

all:
	mkdir -p "root/home/student"
	mkdir -p "root/bin"
	mkdir -p "root/.oldroot"
	mkdir -p "rootfs-target"
	mkdir -p "tmpfs-target"
	mkdir -p "input"
	mkdir -p "output"
	$(CC) $(CFLAGS) $(LDFLAGS) -o input/hej hej.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o cgroups cgroups.c
	$(CC) $(CFLAGS) $(LDFLAGS) -pthread -o user user.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o mnt mnt.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o minpuf minpuf.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o mount-rootfs mount-rootfs.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o pivot-root pivot-root.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o root/bin/.umount-oldroot umount-oldroot.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o root/bin/.setuid setuid.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o root/bin/.rlimits rlimits.c
