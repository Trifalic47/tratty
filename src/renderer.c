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
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        SDL_SetRenderDrawColor(renderer,30,30,30,255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
    conf.status = 0;
    return conf;
}
