//
// Created by user on 2024/1/19.
//
#include <help.h>
static double elapsedSeconds;
static Uint32 tick;
void do_help_logic(){
    app.keyboard[SDL_SCANCODE_SPACE] = false;

    tick = SDL_GetTicks();
    elapsedSeconds = 0.0;

    init_Audio();

    Mix_Music *bgm = Mix_LoadMUS("./res/music/help.mp3");
    Mix_PlayMusic(bgm,0);

    while(!app.keyboard[SDL_SCANCODE_ESCAPE] && next != 0){
        do_help_input();

        draw_help();
    }
    Mix_FreeMusic(bgm);
}

static void do_help_input(){
    SDL_Event event;

    while (SDL_PollEvent(&event)){
        do_event(event);
    }

    if(app.keyboard[SDL_SCANCODE_SPACE]){
        next = 0;
    }

}



static void draw_help(){
    SDL_RenderClear(app.renderer);

    display_image("./res/img/menu_background_1073x600.jpg",0,0);
    display_image("./res/img/help.png",20,60);

    const char* text = "press SPACE to go back to the menu";
    SDL_Color textColor = {0, 102, 0, 255};
    TTF_Font *font = TTF_OpenFont("./res/font/Peralta-Regular.ttf",36);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app.renderer, textSurface);
    double alpha = fabsf((sinf((float)(elapsedSeconds))));
    SDL_SetTextureAlphaMod(textTexture, (int)(255 * alpha));
    SDL_Rect rect = {.x = 30,.y = 450};
    SDL_QueryTexture(textTexture,NULL,NULL,&rect.w,&rect.h);
    SDL_RenderCopy(app.renderer,textTexture,NULL,&rect);
    elapsedSeconds = (SDL_GetTicks() - tick) / 1500.0;

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    SDL_RenderPresent(app.renderer);
}