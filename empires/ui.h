/* Copyright 2016-2018 the Age of Empires Free Software Remake authors. See LEGAL for legal info */

#ifndef UI_H
#define UI_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#ifdef __cplusplus

#include "render.hpp"

extern "C" {
#endif

#include <stdbool.h>

extern SDL_Renderer *renderer;
extern int running;

void ui_init(void);
void ui_free(void);

void idle(unsigned ms);
void display(void);
void repaint(void);

void keydown(SDL_KeyboardEvent *event);
void keyup(SDL_KeyboardEvent *event);

void mousedown(SDL_MouseButtonEvent *event);
void mouseup(SDL_MouseButtonEvent *event);

#ifdef __cplusplus
}

#include <string>

std::string load_string(unsigned id);

#endif

#endif
