#include <stdint.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <pty.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <SDL2/SDL.h>

#define PAGE_SIZE 8192

static int32_t masterfd;

void enableRawMode(void);
