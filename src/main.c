#include <main.h>

#undef main

App app;
int next = -1;


int main(int argc, char *argv[]) {

    init_app();
    init_keyboard();

    app.window = SDL_CreateWindow("Leap And Land",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_RESIZABLE);
    app.renderer = SDL_CreateRenderer(app.window,-1,SDL_RENDERER_ACCELERATED);



    while(!app.keyboard[SDL_SCANCODE_ESCAPE]){
        init_Audio();

        Mix_Music *bgm = Mix_LoadMUS("./res/music/C418 - Minecraft.mp3");
        Mix_PlayMusic(bgm,-1);
        do_menu_logic();

        if(!app.keyboard[SDL_SCANCODE_ESCAPE]){

            Mix_FreeMusic(bgm);

            if(next == 0){
                while(!app.keyboard[SDL_SCANCODE_ESCAPE]){
                    do_game_logic();
                }
            }else{
                do_help_logic();
            }
        }
    }


    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);

    quit_app();

    return 0;
}

static void init_app(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)<0){
        HANDLE_ERROR("Init SDL");
    };
    if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)){
        HANDLE_ERROR("Init Image");
    }
    if(!Mix_Init(MIX_INIT_MP3)){
        HANDLE_ERROR("Init Mixer");
    };
    if(TTF_Init()==-1){
        HANDLE_ERROR("Init TTF");
    };

}

static void quit_app(){
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
}

static void init_keyboard(){
    app.keyboard = calloc(SDL_NUM_SCANCODES, sizeof(bool));
}

