#ifndef CJ_LIB_H
#define CJ_LIB_H

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "cj_vector.h"
#include "cj_network.h"
#include "cj_keycodes.h"
#include "cj_defines.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>





struct WORLD
{
    
};

struct USERINPUT
{
    u32 keyPress[2024];
    u32 keyPressOnce[2024];
};

struct SPRITE
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    
};

struct TEXT
{
    TTF_Font *font;
    char fontPath[100] = {};
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect destRect;
    SDL_Rect srcRect;
    char string[100] = {};
};

struct GAMESTATE
{
    bool running;
    bool console_ON;
    
    V2i mouseCoord;
    char text_input[1024];
    i32 text_input_index;
    bool text_input_happend;
    
    
    
};

struct SDL
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *window_icon;
    
    i32 renderFlags;
    i32 imgFlags;
    
    SDL_Event event;
    
    
};

SDL InitSDL(char *appName, i32 x, i32 y, i32 w, i32 h);

GAMESTATE InitGame(SDL *sdl, GAMESTATE *gs, USERINPUT *userInput, SPRITE *sprite, TEXT *text);

void UpdateGame(SDL *sdl, GAMESTATE *gs, USERINPUT *userInput, SPRITE *sprite, TEXT *text);
void SetDrawColor(SDL *sdl, u8 r, u8 g, u8 b, u8 a);
void ClearScreen(SDL *sdl);
void Present(SDL *sdl);
void SetWindowIcon(SDL *sdl, char *iconPath);
void EventHandler(SDL *sdl, GAMESTATE *gs, USERINPUT *input);


void ToggleConsole(GAMESTATE *gs, USERINPUT *userInput);
void UpdateConsole(GAMESTATE *gs, SPRITE *sprite);

SDL_Rect Rect(i32 x, i32 y, i32 w, i32 h);
SDL_Color Color(i32 r, i32 g, i32 b, i32 a);


void LoadSprite(SDL *sdl, SPRITE *s, char *filePath);
void FreeSprite(SPRITE *sprite);
void DrawSprite(SDL *sdl, SPRITE sprite);
void MoveSprite(SPRITE *s, V2i v);
void SetSpritePos(SPRITE *s, V2i v);
void SetSpriteSize(SPRITE *s, V2i v);
void SetSpriteImagePos(SPRITE *s, V2i v);
void SetSpriteImageSize(SPRITE *s, V2i v);
void SetSpriteColor(SPRITE *s, SDL_Color c);

TEXT LoadFont(char *fontPath, i32 pointSize);
void LoadText(SDL *sdl, TEXT *t, char* text, SDL_Color color);
void DrawText(SDL *sdl, TEXT *t);
void SetTextPos(TEXT *t, V2i v);
void SetTextSize(TEXT *t, V2i v);
void SetTextColor(TEXT *t, SDL_Color c);
i32 GetLongestTextW(TEXT *t, u32 elements_to_search);

bool Collision_AABB_Rect_Rect(SDL_Rect a, SDL_Rect b);
bool Collision_AABB_mouse_rect(GAMESTATE game, SDL_Rect rect);


void GetFilesInDir(char *dir_to_list, char *list_to_fill, i32 size_of_list_element);




#endif // CJ_LIB_H
