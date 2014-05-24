CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu11
LDFLAGS=

all:
	mkdir -p "/root/home/student"
	$(CC) $(CFLAGS) $(LDFLAGS) -o root/hej hej.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o userns userns.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o mnt mnt.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o minpuf minpuf.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o rootfs rootfs.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o pivot-root pivot-root.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o root/init init.c
