#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_quit.h>

#define CENTER SDL_WINDOWPOS_CENTERED

struct SDL2_Renderer {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int status;
};

void SDL2_PRINT_ERROR(bool running);
void SDL2_EXIT(SDL_Window *window,SDL_Renderer *renderer);
struct SDL2_Renderer SDL2_INIT(int x,int y, int h,int w);
