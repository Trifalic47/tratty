#include "../include/core.h"
#include <stdlib.h>


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

int terminal_init(void) {
    pid_t pid;
    pid = forkpty(&masterfd,NULL,NULL,NULL);

    if (pid == -1) {
        perror("forkpty\n");
        return -1;
    } else  if (pid == 0) { 
        // PTY slave (child process)

        execl("/bin/bash","bash",NULL);
        perror("execl");
        return -1;
    } else {
        // PTY master (parent process)

        fd_set readfds;
        while (1) {
            char buff[PAGE_SIZE];

            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO,&readfds);
            FD_SET(masterfd,&readfds);

            int maxfds = (STDIN_FILENO > masterfd)
                ?STDIN_FILENO
                :masterfd;

            if (select(maxfds+1,&readfds,NULL,NULL,NULL) == -1) {

                perror("Select\n");
                return -1;
            }

            if (FD_ISSET(STDIN_FILENO,&readfds)) {
                int c = read(STDIN_FILENO,buff,PAGE_SIZE);
                if (c <= 0) break;

                write(masterfd,buff,c);
            }
            if (FD_ISSET(masterfd,&readfds)) {
                int c = read(masterfd,buff,PAGE_SIZE);
                if (c <= 0) break;

                write(STDOUT_FILENO,buff,c);
            }

        }
    }
    return 0;
}
