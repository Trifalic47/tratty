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

struct TTF_FONT_STRUCT {
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    int status;
};

void SDL2_PRINT_ERROR(bool running);
void SDL2_EXIT(SDL_Window *window,SDL_Renderer *renderer);
struct SDL2_Renderer SDL2_INIT(int x,int y, int h,int w);
void SDL2_BEGIN_FRAME(SDL_Renderer *renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a);
void SDL2_END_FRAME(SDL_Renderer *renderer);
struct TTF_FONT_STRUCT SDL2_TTF_FONT_INIT(char *path,int size);
void TTF_RENDER_FONT(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color,char *text,int x,int y,int w,int h,struct TTF_FONT_STRUCT *font_struct);
