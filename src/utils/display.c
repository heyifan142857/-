//
// Created by user on 2024/1/18.
//
#include <utils/display.h>


void display_image(const char* filePath, int x, int y) {
    SDL_Surface *surface_image = IMG_Load(filePath);
    SDL_Texture *texture_image = SDL_CreateTextureFromSurface(app.renderer, surface_image);
    SDL_Rect rect_image = {.x = x, .y = y};
    SDL_QueryTexture(texture_image, NULL, NULL, &rect_image.w, &rect_image.h);
    SDL_RenderCopy(app.renderer, texture_image, NULL, &rect_image);

    SDL_FreeSurface(surface_image);
    SDL_DestroyTexture(texture_image);
}

void display_font(const char* filePath,const char* text,int size,int r,int g,int b,int x,int y){
    TTF_Font *font = TTF_OpenFont(filePath,size);
    SDL_Color color = {r,g,b};
    SDL_Surface *surface = TTF_RenderText_Blended(font,text,color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,surface);
    SDL_Rect rect = {.x = x,.y = y};
    SDL_QueryTexture(texture,NULL,NULL,&rect.w,&rect.h);
    SDL_RenderCopy(app.renderer,texture,NULL,&rect);
    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
}
