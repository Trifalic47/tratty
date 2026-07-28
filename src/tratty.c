#include "../include/core.h"
#include "../include/renderer.h"
#include "../include/screen.h"

const SDL_Color WHITE = {255,255,255,255};

int main(int argc, char *argv[])
{
    struct termios origional;
    tcgetattr(STDIN_FILENO,&origional);

    enableRawMode();

    struct SDL2_Renderer conf;
    conf = SDL2_INIT(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,700,1200); 
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

    int cursor_row = 0;
    int cursor_col = 0;

    while (running) {
        char buff[PAGE_SIZE];
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            else if (event.type == SDL_KEYDOWN) {
                char c = (char)event.key.keysym.sym;
                write(terminal.masterfd,&c,1);
            } else {
                continue;
            }
        }

        FD_ZERO(&readfds);
        FD_SET(terminal.masterfd,&readfds);

        struct timeval tv = {
            .tv_sec = 0,
            .tv_usec = 10000,
        };

        if (select(terminal.masterfd +1,&readfds,NULL,NULL,&tv) == -1) {
            tcsetattr(STDIN_FILENO,TCSANOW,&origional);
            perror("select\n");
            return EXIT_FAILURE;
        }

        if (FD_ISSET(terminal.masterfd,&readfds)) {
            ssize_t n = read(terminal.masterfd,buff,PAGE_SIZE);
            if (n <= 0) break;
            for (ssize_t i =0; i<n;i++) {
                screen[cursor_row][cursor_col].ch = buff[i];
                cursor_col++;
            }
        }
        SDL2_BEGIN_FRAME(conf.renderer,0,0,0,255);
        for (int row = 0;row<ROWS;row++) {
            for (int col = 0; col < COLS;col++) {
                if (screen[row][col].ch == '\0') continue;
                char text[2] = {
                    screen[row][col].ch,
                    '\0',
                };
                TTF_RENDER_FONT(conf.renderer,font.font,WHITE,text,col*20,row*40,20,40);
            }
        };
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
