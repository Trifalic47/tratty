#include "../include/core.h"

int main(int argc, char *argv[])
{
    char *param = get_param("\033[2J",1,strlen("\033[2J")-1);
    printf("%s\n",param);
    free(param);
    return EXIT_SUCCESS;
}
