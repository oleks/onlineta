CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu11
LDFLAGS=

all:
	mkdir -p "root/home/student"
	mkdir -p "root/.oldroot"
	mkdir -p "rootfs-target"
	mkdir -p "tmpfs-target"
	mkdir -p "input"
	mkdir -p "output"
	$(CC) $(CFLAGS) $(LDFLAGS) -o input/hej hej.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o userns userns.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o mnt mnt.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o minpuf minpuf.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o rootfs rootfs.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o pivot-root pivot-root.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o root/init init.c
