#include <stdint.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <pty.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>

#define PAGE_SIZE 8192

struct init_struct {
    pid_t pid;
    int32_t masterfd;
    int status;
};

void enableRawMode(void);
struct init_struct terminal_init(void);
