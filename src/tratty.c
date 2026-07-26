#include "../include/core.h"

int main(int argc, char *argv[])
{
    struct termios origional;
    tcgetattr(STDIN_FILENO,&origional);

    enableRawMode();
    pid_t pid;
    pid = forkpty(&masterfd,NULL,NULL,NULL);

    if (pid == -1) {
        tcsetattr(STDIN_FILENO,TCSANOW,&origional);
        perror("forkpty\n");
        exit(EXIT_FAILURE);
    } else  if (pid == 0) { 
        // PTY slave (child process)

        execl("/bin/bash","bash",NULL);
        perror("execl");
        exit(EXIT_FAILURE);
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

                tcsetattr(STDIN_FILENO,TCSANOW,&origional);
                perror("Select\n");
                exit(EXIT_FAILURE);
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
    tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    return EXIT_SUCCESS;
}
