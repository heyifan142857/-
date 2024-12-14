//
// Created by user on 2024/1/18.
//
#include <utils/audio.h>

void init_Audio() {
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
        printf("SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }
}

void do_music(const char* filePath, int loop,Mix_Music *bgm){
    bgm = Mix_LoadMUS(filePath);
    Mix_PlayMusic(bgm,loop);
}

