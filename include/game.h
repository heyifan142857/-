//
// Created by user on 2024/1/17.
//

#ifndef LEAP_AND_LAND_GAME_H
#define LEAP_AND_LAND_GAME_H

#include <common.h>
#include <utils/input.h>
#include <utils/display.h>
#include <kun.h>

#define FPS 90
#define FRAME_LEN (1000 / FPS)
#define KUN_SPEED 130.0

#define LEN_CHUNK 2371.0
#define NUM_BLOCKS 7
#define WIDTH 100

typedef struct node{
    SDL_Texture *texture;
    int shape;
    double x;
    int size;
    int len;
    struct node *before;
    struct node *after;
}Blocks;


typedef struct list{
    Blocks *head;
    Blocks *middle;
    Blocks *tail;
}LinkedList;

void do_game_logic();
static void do_game_input();
static void do_auto();
static void do_score();
static void do_Moist();
static void draw_game();
static void do_fps();
static double speed_calculator_h(Uint32 time);
static int speed_calculator_y(int type,int size);
static void init_blocks();
static void quit_blocks();
void draw_blocks(LinkedList list);
static char *generateRandomNumber();
static double difficulty_random_len(int diff);
static double difficulty_random_size(int diff);
extern bool fail;



#endif //LEAP_AND_LAND_GAME_H
