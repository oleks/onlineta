# vim: set noexpandtab:

CC=clang
CFLAGS=#-Wall -Wextra -pedantic -std=gnu11
LDFLAGS=#-lmount

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o sysexec sysexec.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o libexec libexec.c
