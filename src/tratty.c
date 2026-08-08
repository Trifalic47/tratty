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

    char buff[PAGE_SIZE];
    while (running) {
        SDL_Event event;

        SDL2_BEGIN_FRAME(conf.renderer,0,0,0,255);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym){
                    case SDLK_BACKSPACE: {
                                             char c = 0x7F;
                                             write(terminal.masterfd,&c,1);
                                             break;
                                         }
                    case SDLK_RETURN:
                                         write(terminal.masterfd,"\n",1);
                                         break;
                    case SDLK_TAB:
                                         write(terminal.masterfd,"\t",1);
                                         break;
                    case SDLK_ESCAPE:{
                                         char c = 0x1B;
                                         write(terminal.masterfd,&c,1);
                                         break;
                                     }
                    case SDLK_UP:
                                     printf("Up detected\n");
                                     char c[3] = {0x1B,0x5B,0x41};
                                     write(terminal.masterfd,c,3);
                                     break;
                    case SDLK_g:{
                                    if (event.key.keysym.mod & KMOD_CTRL) {
                                        char c = 0x0A;
                                        write(terminal.masterfd,&c,1);
                                        printf("\a");
                                        fflush(STDIN_FILENO);
                                    }
                                    break;
                                }
                    case SDLK_l:{
                                    if (event.key.keysym.mod && KMOD_CTRL) {
                                        char ascii = (unsigned char)12;
                                        write(terminal.masterfd,&ascii,1);
                                    }
                                    break;
                                }
                    default:
                                break;
                }
            }
            if (event.type == SDL_TEXTINPUT) {
                write(terminal.masterfd,event.text.text,strlen(event.text.text));
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
                // printf("input: %c \t ASCII: %d \t hex: %02x \n",buff[i],(unsigned char)buff[i],(unsigned char)buff[i]);
                if (buff[i] == 0x1B) {
                    if (buff[i+1] == 0x5B) {
                        if (buff[i+2] == 0x41) {
                            // Move up
                        }
                        if (buff[i+2] == 0x32) {
                            if(buff[i+3] == 0x4A) {
                                // clear -> ^L
                                cursor_col = 0;
                                cursor_row = 0;
                                SDL2_BEGIN_FRAME(conf.renderer,0,0,0,255);
                                memset(screen,0,sizeof(screen));
                            }
                        }
                    }
                } else if (buff[i] == '\b' || buff[i] == 0x08 || buff[i] == 0x07) {
                    if (cursor_col > 0)
                        cursor_col--;
                } else if (buff[i] == '\n') {
                    cursor_col = 0;
                    cursor_row++;
                }
                else if (buff[i] == '\t') {
                    int next_tab = (cursor_col + 8) & ~7;
                    while (cursor_col < next_tab || cursor_col < COLS) {
                        screen[cursor_row][cursor_col].ch = ' ';
                        cursor_col++;
                    }
                }
                if (isprint((unsigned char)buff[i])) {
                    screen[cursor_row][cursor_col].ch = buff[i];
                    cursor_col++;
                }
            }
        }
        for (int row = 0;row<ROWS;row++) {
            for (int col = 0; col < COLS;col++) {
                if (screen[row][col].ch == '\0') continue;
                char text[2] = {
                    screen[row][col].ch,
                    '\0',
                };
                TTF_RENDER_FONT(conf.renderer,font.font,WHITE,text,col*20,row*40,20,40,&font);
            }
        };
        SDL2_END_FRAME(conf.renderer);
    }

    tcsetattr(STDIN_FILENO,TCSANOW,&origional);
    SDL_DestroyRenderer(conf.renderer);
    SDL_DestroyWindow(conf.window);
    TTF_CloseFont(font.font);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
