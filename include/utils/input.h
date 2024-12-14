//
// Created by user on 2024/1/16.
//

#ifndef LEAP_AND_LAND_INPUT_H
#define LEAP_AND_LAND_INPUT_H

#include <common.h>

void do_event(SDL_Event event);

static void do_quit(SDL_QuitEvent quit);

static void do_keydown(SDL_KeyboardEvent key);
static void do_keyup(SDL_KeyboardEvent key);

static void do_mousebuttondown(SDL_MouseButtonEvent key);
static void do_mousebuttonup(SDL_MouseButtonEvent key);

#endif //LEAP_AND_LAND_INPUT_H
