#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pty.h>
#include <unistd.h>

static int32_t masterfd;

int main(int argc, char *argv[])
{
  pid_t pid;

  pid = forkpty(&masterfd,NULL,NULL,NULL);

  if (pid == -1) {
    perror("forkpty");
    return EXIT_FAILURE;
  } else if ( pid == 0) {
    execl("/bin/bash", "bash", NULL);
    perror("execl");
    exit(EXIT_FAILURE);
  } else {
    char buff[256];
    while (1) {
      int n = read(masterfd,buff,256);
      if (n <= 0) break;

      write(STDOUT_FILENO,buff,n);
    }
  }
  return EXIT_SUCCESS;
}
