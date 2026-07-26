#include "../include/core.h"


void enableRawMode(void) {
    struct termios changed;     
    if (tcgetattr(STDIN_FILENO, &changed) == -1) {
        perror("tcsetattr\n");
        exit(EXIT_FAILURE);
    };
    changed.c_lflag &= ~(ICANON | ECHO | ISIG);
    if (tcsetattr(STDIN_FILENO,TCSANOW,&changed) == -1) {
        perror("tcsetattr\n");
        exit(EXIT_FAILURE);
    }
}
