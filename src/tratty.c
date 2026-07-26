#include "../include/core.h"
#include "../include/renderer.h"

int main(int argc, char *argv[])
{
    struct termios origional;
    tcgetattr(STDIN_FILENO,&origional);

    enableRawMode();

    if (terminal_init() == -1) {
        perror("terminal_init\n");
        tcsetattr(STDIN_FILENO,TCSANOW,&origional);
        return EXIT_FAILURE;
    }

    struct SDL2_Renderer conf;
    conf = SDL2_INIT(CENTER,CENTER,600,800);
    if (conf.status == -1) {
        tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    }
    SDL_DestroyWindow(conf.window);
    SDL_DestroyRenderer(conf.renderer);
    tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    return EXIT_SUCCESS;
}
