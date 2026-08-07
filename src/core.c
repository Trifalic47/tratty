#include "../include/core.h"

void enableRawMode(void) {
    struct termios changed;
    if (tcgetattr(STDIN_FILENO, &changed) == -1) {
        perror("tcsetattr\n");
        exit(-1);
    };
    changed.c_lflag &= ~(ICANON | ECHO | ISIG);
    if (tcsetattr(STDIN_FILENO,TCSANOW,&changed) == -1) {
        perror("tcsetattr\n");
        exit(-1);
    }
}

struct init_struct terminal_init(void) {
    struct init_struct init;
    pid_t pid;
    pid = forkpty(&init.masterfd,NULL,NULL,NULL);

    init.pid = pid;

    if (pid == -1) {
        perror("forkpty\n");
        init.status = -1;
        return init;
    } else  if (pid == 0) {
        // PTY slave (child process)

        execl("/bin/bash","bash",NULL);
        perror("execl");
        init.status = -1;
        return init;
    } else {
        // PTY master (parent process)
        init.status = 0;
    }
    return init;
}

