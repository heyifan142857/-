//
// Created by user on 2024/1/18.
//

#ifndef LEAP_AND_LAND_KUN_H
#define LEAP_AND_LAND_KUN_H

#include <common.h>
#include <utils/display.h>

#define KUN_INIT_H 0
#define KUN_ACCELERATION 300.0
#define ANIMATION_DELAY 40

typedef struct{
    SDL_Texture *texture;
    double h;
    double dh;
} Kun;



void init_kun(Kun *kun);
void do_kun(Kun kun,Uint32 time);
void draw_kun(Kun *kun);
void quit_kun(Kun *kun);


#endif //LEAP_AND_LAND_KUN_H
