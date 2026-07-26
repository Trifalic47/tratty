#include "../include/core.h"
#include "../include/renderer.h"
#include <SDL2/SDL_ttf.h>

const SDL_Color WHITE = {255,255,255,255};

int main(int argc, char *argv[])
{
    struct termios origional;
    tcgetattr(STDIN_FILENO,&origional);

    enableRawMode();

    // if (terminal_init() == -1) {
    //     perror("terminal_init\n");
    //     tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    //     return EXIT_FAILURE;
    // }

    struct SDL2_Renderer conf;
    conf = SDL2_INIT(CENTER,CENTER,600,800);
    if (conf.status == -1) {
        tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    }

    struct TTF_FONT_STRUCT font;
    font = SDL2_TTF_FONT_INIT("assets/fonts/JetBrains/JetBrainsMonoNerdFont-Regular.ttf",40);
    if (font.status == -1) {
        tcsetattr(STDIN_FILENO,TCSANOW,&origional);
        perror("font init failed\n");
        return EXIT_FAILURE;
    }
    
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        SDL2_BEGIN_FRAME(conf.renderer,30,30,30,255);
        TTF_RENDER_FONT(conf.renderer,font.font,WHITE,"Hello, World!",100,100,400,100);
        SDL2_END_FRAME(conf.renderer);
    }

    SDL_DestroyRenderer(conf.renderer);
    SDL_DestroyWindow(conf.window);
    TTF_CloseFont(font.font);
    TTF_Quit();
    SDL_Quit();


    tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    return EXIT_SUCCESS;
}
