#include "cj_lib.h"


SDL InitSDL(char *appName, i32 x, i32 y, i32 w, i32 h)
{
    SDL result = {};
    
    result.window = SDL_CreateWindow(appName,
                                     x, y,
                                     w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    
    result.renderFlags = SDL_RENDERER_PRESENTVSYNC;
    result.renderer = SDL_CreateRenderer(result.window, -1, result.renderFlags);
    
    result.imgFlags = IMG_INIT_PNG;
    if ( !(IMG_Init(result.imgFlags) & result.imgFlags) ) { printf("IMG_Init failed: %s\n", IMG_GetError());}
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    
    
    
    
    return result;
}



void SetDrawColor(SDL *sdl, u8 r, u8 g, u8 b, u8 a)
{
    SDL_SetRenderDrawColor(sdl->renderer, r, g, b, a);
}

void ClearScreen(SDL *sdl)
{
    SDL_RenderClear(sdl->renderer);
}

void Present(SDL *sdl)
{
    SDL_RenderPresent(sdl->renderer);
}

void SetWindowIcon(SDL *sdl, char *iconPath)
{
    sdl->window_icon = SDL_LoadBMP(iconPath);
    if(!sdl->window_icon)
        printf("couldn't load the .bmp from %s\n", iconPath);
    else
    {
        SDL_SetWindowIcon(sdl->window, sdl->window_icon);
    }
    
}

void EventHandler(SDL *sdl, GAMESTATE *gs, USERINPUT *input)
{
    
    
    while(SDL_PollEvent(&sdl->event) != 0)
    {
        
        switch(sdl->event.type)
        {
            case SDLK_ESCAPE:
            {
                gs->running = false;
            } break;
            
            case SDL_QUIT:
            {
                gs->running = false;
            } break;
            
            case SDL_TEXTINPUT:
            {
                if(gs->text_input_index < 1024)
                {
                    
                    gs->text_input[gs->text_input_index] = sdl->event.text.text[0];
                    gs->text_input_index++;
                    gs->text_input_happend = true;
                }
            } break;
            
            case SDL_KEYDOWN:
            {
                SDL_Keycode keycode = sdl->event.key.keysym.sym;
                
                if( keycode == SDLK_APPLICATION) input->keyPress[windowsKey] = true;
                if( keycode == SDLK_UP)  input->keyPress[up] = true;
                if( keycode == SDLK_DOWN)  input->keyPress[down] = true; 
                if( keycode == SDLK_LEFT)  input->keyPress[left] = true;
                if( keycode == SDLK_RIGHT)  input->keyPress[right] = true; 
                if( keycode == SDLK_BACKSPACE)  
                {
                    input->keyPress[backspace] = true; 
                    
                    gs->text_input_index--;
                    
                    if(gs->text_input_index < 0)
                        gs->text_input_index = 0;
                    
                    gs->text_input[gs->text_input_index] = ' ';
                    gs->text_input_happend = true;
                }
                
                // Checking ASCII
                if(keycode > 0 && keycode < 1024)
                {
                    input->keyPress[keycode] = true;
                    
                    if(sdl->event.key.repeat == 0) 
                    { 
                        input->keyPressOnce[keycode] = true;
                        //printf("input->keyPressOnce true\n");
                    }
                    else
                    {
                        input->keyPressOnce[keycode] = false; 
                        //printf("input->keyPressOnce false\n");
                    }
                    
                }
                
                if(sdl->event.key.repeat == 0)
                {
                    if( keycode == SDLK_APPLICATION) input->keyPressOnce[windowsKey] = true;
                    if( keycode == SDLK_UP)  input->keyPressOnce[up] = true;
                    if( keycode == SDLK_DOWN)  input->keyPressOnce[down] = true; 
                    if( keycode == SDLK_LEFT)  input->keyPressOnce[left] = true;
                    if( keycode == SDLK_RIGHT)  input->keyPressOnce[right] = true; 
                    if( keycode == SDLK_BACKSPACE)  input->keyPressOnce[backspace] = true; 
                    
                }
                else
                {
                    if( keycode == SDLK_APPLICATION) input->keyPressOnce[windowsKey] = false;
                    if( keycode == SDLK_UP)  input->keyPressOnce[up] = false;
                    if( keycode == SDLK_DOWN)  input->keyPressOnce[down] = false; 
                    if( keycode == SDLK_LEFT)  input->keyPressOnce[left] = false;
                    if( keycode == SDLK_RIGHT)  input->keyPressOnce[right] = false; 
                    if( keycode == SDLK_BACKSPACE)  input->keyPressOnce[backspace] = false; 
                    
                }
                
                
            } break;
            
            case SDL_KEYUP:
            {
                SDL_Keycode keycode = sdl->event.key.keysym.sym;
                
                if( keycode == SDLK_UP) { input->keyPress[up] = false; }
                if( keycode == SDLK_DOWN) { input->keyPress[down] = false; }
                if( keycode == SDLK_LEFT)  input->keyPress[left] = false;
                if( keycode == SDLK_RIGHT)  input->keyPress[right] = false; 
                if( keycode == SDLK_BACKSPACE)  input->keyPress[backspace] = false; 
                if(keycode == SDLK_APPLICATION)
                    input->keyPress[windowsKey] = false;
                
                if(keycode > 0 && keycode < 1024)
                {
                    input->keyPress[keycode] = false;
                    input->keyPressOnce[keycode] = false;
                }
                
            } break;
            
            case SDL_MOUSEBUTTONDOWN:
            {
                if(SDL_BUTTON_LEFT)
                {
                    
                    input->keyPress[mouse_L] = true;
                    
                    if(sdl->event.key.repeat) 
                    { 
                        input->keyPressOnce[mouse_L] = false; 
                        printf("input->keyPressOnce false\n");
                    }
                    else 
                    {
                        input->keyPressOnce[mouse_L] = true;
                        printf("input->keyPressOnce true\n");
                    }
                    
                }
                
            } break;
            
            case SDL_MOUSEBUTTONUP:
            {
                if(SDL_BUTTON_LEFT)
                {
                    input->keyPress[mouse_L] = false;
                    input->keyPressOnce[mouse_L] = false; 
                }
                
            } break;
            
        }
    }
}



void ToggleConsole(GAMESTATE *gs, USERINPUT *userInput)
{
    if(userInput->keyPressOnce['c'])
    { 
        userInput->keyPressOnce['c'] = false;
        if(!gs->console_ON) 
        { 
            gs->console_ON = true; 
        } 
        else // Close Console 
        {
            gs->console_ON = false; 
            
        }
    }
    
}

void UpdateConsole(GAMESTATE *gs, SPRITE *sprite)
{
    if(gs->console_ON) 
    { 
        if(sprite->destRect.y <= -4)
        {
            MoveSprite(sprite, v2i(0, 20));
        }
        
        
    }
    else
    {
        if(sprite->destRect.y >= -5*64)
        {
            MoveSprite(sprite, v2i(0, -20));
        }
    }
    
}

void LoadSprite(SDL *sdl, SPRITE *s, char *filePath)
{
    if(s->texture)
    {
        SDL_DestroyTexture(s->texture);
    }
    
    s->surface = IMG_Load(filePath);
    if(!s->surface) { printf("can't load surface: %s\n error: %s\n", filePath, IMG_GetError()); }
    else
    {
        s->texture = SDL_CreateTextureFromSurface(sdl->renderer, s->surface);
        if(!s->texture) {printf("can't load texture: %s\n error: %s\n", filePath, IMG_GetError());}
    }
    
    SDL_FreeSurface(s->surface);
    
}

void FreeSprite(SPRITE *sprite)
{
    SDL_DestroyTexture(sprite->texture);
}

void DrawSprite(SDL *sdl, SPRITE sprite)
{
    SDL_RenderCopy(sdl->renderer, sprite.texture, &sprite.srcRect, &sprite.destRect);
}

void MoveSprite(SPRITE *s, V2i v)
{
    s->destRect.x += v.x;
    s->destRect.y += v.y;
}

void SetSpritePos(SPRITE *s, V2i v)
{
    s->destRect.x = v.x;
    s->destRect.y = v.y;
}

void SetSpriteSize(SPRITE *s, V2i v)
{
    s->destRect.w = v.x;
    s->destRect.h = v.y;
}

void SetSpriteImagePos(SPRITE *s, V2i v)
{
    s->srcRect.x = v.x;
    s->srcRect.y = v.y;
}

void SetSpriteImageSize(SPRITE *s, V2i v)
{
    s->srcRect.w = v.x;
    s->srcRect.h = v.y;
}

void SetSpriteColor(SPRITE *s, SDL_Color c)
{
    SDL_SetTextureColorMod(s->texture, c.r, c.g, c.b);
}

SDL_Rect Rect(i32 x, i32 y, i32 w, i32 h)
{
    SDL_Rect result = {};
    
    result.x = x;
    result.y = y;
    result.w = w;
    result.h = h;
    
    return result;
}

SDL_Color Color(i32 r, i32 g, i32 b, i32 a)
{
    SDL_Color result = {};
    result.r = r;
    result.g = g;
    result.b = b;
    result.a = a;
    
    return result;
}

TEXT LoadFont(char *fontPath, i32 pointSize)
{
    TEXT result = {};
    //Open the font
    
    result.font = TTF_OpenFont( fontPath, pointSize );
    if( !result.font )
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    
    
    return result;
}

void LoadText(SDL *sdl, TEXT *t, char* text, SDL_Color color)
{
    if(t->texture) { SDL_DestroyTexture(t->texture); }
    
    t->surface = TTF_RenderText_Solid(t->font, text, color);
    if(!t->surface) { printf("Couldn't load: %s\nError: %s\n", t->string, TTF_GetError()); }
    else
    {
        t->texture = SDL_CreateTextureFromSurface(sdl->renderer, t->surface);
        if(!t->texture) { printf("Couldn't make texture from surface\nError: %s\n", TTF_GetError()); }
        else
        {
            t->destRect.x = 0;
            t->destRect.y = 0;
            t->destRect.w = t->surface->w;
            t->destRect.h = t->surface->h;
            
        }
    }
    
    SDL_FreeSurface(t->surface);
    
    
    
}

void DrawText(SDL *sdl, TEXT *t)
{
    SDL_RenderCopy(sdl->renderer, t->texture, 0, &t->destRect);
}

void SetTextPos(TEXT *t, V2i v)
{
    t->destRect.x = v.x;
    t->destRect.y = v.y;
}

void SetTextSize(TEXT *t, V2i v)
{
    t->destRect.w = v.x;
    t->destRect.h = v.y;
}

void SetTextColor(TEXT *t, SDL_Color c)
{
    SDL_SetTextureColorMod(t->texture, c.r, c.g, c.b);
}

i32 GetLongestTextW(TEXT *t, u32 elements_to_search)
{
    i32 textW_max = 0;
    i32 textW_temp = 0;
    
    for(i32 i = 0; i < elements_to_search; i++)
    {
        textW_temp = t[i].destRect.w; 
        if(textW_max < textW_temp)
            textW_max = textW_temp;
        
        
    }
    
    return textW_max;
}

bool Collision_AABB_Rect_Rect(SDL_Rect a, SDL_Rect b)
{
    bool result = false;
    i32 a_Left = a.x;
    i32 a_Right = a.x + a.w;
    i32 a_Top = a.y;
    i32 a_Bottom = a.y + a.h;
    
    i32 b_Left = b.x;
    i32 b_Right = b.x + b.w;
    i32 b_Top = b.y;
    i32 b_Bottom = b.y + b.h;
    
    if(a_Left < b_Right && 
       a_Right > b_Left &&
       a_Bottom > b_Top &&
       a_Top < b_Bottom)
    {
        return true;
    }
    else return false;
}

bool Collision_AABB_mouse_rect(GAMESTATE game, SDL_Rect rect)
{
    bool result = false;
    
    i32 mouse_x = game.mouseCoord.x;
    i32 mouse_y = game.mouseCoord.y;
    
    
    if(mouse_x > rect.x && 
       mouse_x < rect.x + rect.w &&
       mouse_y > rect.y &&
       mouse_y < rect.y + rect.h)
    {
        return true;
    }
    else return false;
}




void GetFilesInDir(char *dir_to_list, char *list_to_fill, i32 size_of_list_element)
{
    
    //char dir_to_list[64] = "assets";
    char dir_ff[64] = {};
    char file_list[10][256] = {};
    i32 file_list_index = 0;
    i32 nfiles_in_list = 0;
    
    WIN32_FIND_DATA win32_ff = {};
    strcat(dir_ff, dir_to_list);
    strcat(dir_ff, "\\*");
    
    HANDLE h_find = FindFirstFile(dir_ff, &win32_ff);
    strcat(file_list[file_list_index], dir_to_list);
    file_list_index++;
    
    if(h_find != INVALID_HANDLE_VALUE)
    {
        while(FindNextFile(h_find, &win32_ff))
        {
            // Check wether the file is a <DIR> or not
            if(win32_ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                strcat(file_list[file_list_index], win32_ff.cFileName);
                
            }
            else
            {
                strcat(file_list[file_list_index], win32_ff.cFileName);
                
            }
            
            file_list_index++;
            
        }
        
        for(i32 i = 0; i < file_list_index; i++)
        {
            printf("file_list[%d]:%s\n", i, file_list[i]);
            
            strcat(list_to_fill, file_list[i]);
            list_to_fill += size_of_list_element;
        }
    }
    
    FindClose(h_find);
    
}



