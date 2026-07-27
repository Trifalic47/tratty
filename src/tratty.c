#include "../include/core.h"
#include "../include/renderer.h"
#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <unistd.h>

const SDL_Color WHITE = {255,255,255,255};

int main(int argc, char *argv[])
{
    struct termios origional;
    tcgetattr(STDIN_FILENO,&origional);

    enableRawMode();

    struct SDL2_Renderer conf;
    conf = SDL2_INIT(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,600,800); 
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
    
    struct init_struct terminal = terminal_init();
    if (terminal.status == -1) {
        tcsetattr(STDIN_FILENO,TCSANOW,&origional);
        perror("Terminal init failed\n");
        return EXIT_FAILURE;
    }
    fd_set readfds;

    bool running = true;
    while (running) {
        char buff[PAGE_SIZE];
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        FD_ZERO(&readfds);
        FD_SET(terminal.masterfd,&readfds);

        int max_fds = (STDIN_FILENO > terminal.masterfd)
            ?STDIN_FILENO
            :terminal.masterfd;

        struct timeval tv = {
            .tv_sec = 0,
            .tv_usec = 1000,
        };

        if (select(max_fds +1,&readfds,NULL,NULL,&tv) == -1) {
            tcsetattr(STDIN_FILENO,TCSANOW,&origional);
            perror("select\n");
            return EXIT_FAILURE;
        }



        if (FD_ISSET(STDIN_FILENO,&readfds)) {
            int c = read(STDIN_FILENO,buff,PAGE_SIZE);
            if (c <= 0) break;

            write(terminal.masterfd,buff,c);
        }
        if (FD_ISSET(terminal.masterfd,&readfds)) {
            int c = read(terminal.masterfd,buff,PAGE_SIZE);
            if (c <= 0) break;

            write(STDOUT_FILENO,buff,c);
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
