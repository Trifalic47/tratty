#include "../include/renderer.h"
#include "../include/core.h"

void SDL2_PRINT_ERROR(bool running) {
    printf("%s\n",SDL_GetError());
    if (running == false) exit(EXIT_FAILURE);
}

struct SDL2_Renderer SDL2_INIT(int x,int y, int h,int w) {
    struct SDL2_Renderer conf;
    conf.window = NULL;
    conf.renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL2_PRINT_ERROR(false);
    }
    SDL_Window *window = SDL_CreateWindow("tratty",x,y,w,h,SDL_WINDOW_SHOWN);
    if (window == NULL) {
        conf.status = -1;
        SDL2_PRINT_ERROR(true);
        return conf;
    }
    conf.window = window;
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        conf.status = -1;
        SDL_DestroyWindow(window);
        SDL2_PRINT_ERROR(true);
        return conf;
    }
    conf.renderer = renderer;
    conf.status = 0;
    return conf;
}

void SDL2_BEGIN_FRAME(SDL_Renderer *renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a) {
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
    SDL_RenderClear(renderer);
}

struct TTF_FONT_STRUCT SDL2_TTF_FONT_INIT(char *path,int size) {
    struct TTF_FONT_STRUCT fconf;
    if (TTF_Init() == -1) {
        fconf.status = -1;
        printf("%s\n",TTF_GetError());
        return fconf;
    }
    TTF_Font *font = TTF_OpenFont(path,size);
    if (!font) {
        fconf.status = -1;
        printf("%s\n",TTF_GetError());
        return fconf;
    }
    fconf.font = font;
    fconf.status = 0;
    return fconf;
}

void SDL2_END_FRAME(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}

void TTF_RENDER_FONT(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color,char *text,int x,int y,int w,int h) {
    SDL_Surface *surface = TTF_RenderText_Solid(font,text,color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect rect = {
        .h = h,
        .w = w,
        .x = x,
        .y = y,
    };
    SDL_RenderCopy(renderer,texture,NULL,&rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

