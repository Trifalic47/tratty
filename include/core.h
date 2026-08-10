#include <stdint.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <pty.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define PAGE_SIZE 4096

struct init_struct {
    pid_t pid;
    int32_t masterfd;
    int status;
};

void enableRawMode(void);
struct init_struct terminal_init(void);
char * get_param(char *ansi,int start_idx,int final_idx);
