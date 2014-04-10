// A lightweight program to start a program in a control group.
// Usage: sysexec <path-to-tasks-file> <executable> <arguments>.
// Unlike libexec, this process does not use the standard library.

#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#define PID_SIZE ((sizeof(pid_t) * CHAR_BIT) / 3)
// ceil((sizeof(pid_t) * CHAR_BIT) / floor(log2(10)))

int nitoa(int val, int base, char *buf, int n)
{

// Inspired by http://www.strudel.org.uk/itoa/

  int __val;
  int ret;

  if (base < 2 || base > 16) return -1; // unsupported base.

  if (val < 0) return -1; // negative values not supported.

  if (n < 2) return -1; // not enough space for a symbol and \0.

  ret = n;
  __val = val;
  do { ++buf; --n; __val /= base; } while (__val && n); // allocate bytes.
  ret -= n;

  if(n < 1) return -1; // no space for \0.

  *(buf--) = '\0';

  do
  {
    *(buf--) = "0123456789abcdef"[val % base];
    val /= base;
  }
  while (val);

  return ret;
}

int main(int argc, char* argv[])
{
  pid_t pid;
  int fd, pid_size;
  char pid_buf[PID_SIZE + 1];

  if (sizeof(int) != sizeof(pid_t))
  {
    return 2;
  }

  if (argc < 3)
  {
    return 1;
  }

  pid = getpid();

  if (pid < 0)
  {
    return 2;
  }

  pid_size = nitoa(pid, 10, pid_buf, PID_SIZE + 1);

  fd = open(argv[1], O_WRONLY | O_APPEND | O_CLOEXEC);
  if (fd == -1)
  {
    return 2;
  }

  if (write(fd, pid_buf, pid_size) != pid_size)
  {
    return 2;
  }

  if (close(fd) == -1)
  {
    return 2;
  }

  execve(argv[2], argv + 3, NULL);

  return 0;
}
