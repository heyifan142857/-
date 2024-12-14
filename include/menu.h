//
// Created by user on 2024/1/16.
//

#ifndef LEAP_AND_LAND_MENU_H
#define LEAP_AND_LAND_MENU_H

#define NUM_WIDGETS 3
#define WIDGET_X 450
#define WIDGET_Y_TOP 270
#define WIDGET_Y_GAP 80

#include <common.h>
#include <utils/input.h>
#include <utils/display.h>

typedef struct {
    const char *text;
    int x, y;
    void (*action)();
} Widget;





void do_menu_logic();
static void do_menu_input(SDL_Event event);
static void draw_menu();
static void init_widgets();
static void quit_widgets();
static void do_widgets();
static void pre_widget();
static void next_widget();
static void act_widget();
static void action_start();
static void action_help();
static void action_quit();

#endif //LEAP_AND_LAND_MENU_H
