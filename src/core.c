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

char * get_param(char *ansi,int start_idx,int final_idx) {
    char final_byte = ansi[strlen(ansi)-1];
    char *param = (char*)malloc((final_idx-start_idx)+1);
    // printf("start_idx:%d , final_idx: %d , param_size: %d\n",start_idx,final_idx,(final_idx-start_idx)+1);

    int j = 0;
    for (int i = start_idx;i <= final_idx;i++) {
        param[j] = ansi[i];
        j++;
    }

    return param;
}
