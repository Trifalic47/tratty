#include <stdio.h>
#include <sys/select.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pty.h>
#include <unistd.h>
#include <termios.h>

#define PAGE_SIZE 8192

static int32_t masterfd;

void enableRawMode(void) {
    struct termios setting;
    if (tcgetattr(STDIN_FILENO, &setting) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    setting.c_lflag &= ~(ICANON | ECHO | ISIG);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &setting) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    // Getting the default/current termios settings (canonical mode is enabled)
    struct termios origional;
    tcgetattr(STDIN_FILENO,&origional);

    enableRawMode();
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
        fd_set readfds;
        while (1){
            FD_ZERO(&readfds);

            char buff[PAGE_SIZE];
            FD_SET(STDIN_FILENO,&readfds);
            FD_SET(masterfd,&readfds);

            int maxfd = (STDIN_FILENO > masterfd)
                ?STDIN_FILENO
                :masterfd;
            int ready = select(maxfd + 1,&readfds,NULL,NULL,NULL);
            if (ready == -1) {  
                perror("select");
                break;
            }
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                int c = read(STDIN_FILENO,buff,PAGE_SIZE);
                if (c <= 0) break;
                write(masterfd,buff,c);
            }
            if (FD_ISSET(masterfd,&readfds)) {
                int c = read(masterfd,buff,PAGE_SIZE);

                if (c <= 0) break;
                for (int i = 0; i < c; i++) {
                    unsigned char ch = buff[i];

                    if (ch == 0x1B) {
                        printf("\nESC detected at index %d\n", i);
                    }

                    printf("%02X ", ch);
                }
                printf("\n");
                write(STDOUT_FILENO,buff,c);
            }
        }
    }
    tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    return EXIT_SUCCESS;
}
