/* Copyright 2018 the Age of Empires Free Software Remake authors. See LEGAL for legal info */

/**
 * Audio subsystem
 *
 * Licensed under Affero General Public License v3.0
 * Copyright by Folkert van Verseveld.
 *
 */

#ifndef SFX_H
#define SFX_H

#include <AL/al.h>
#include <AL/alc.h>
#include <SDL2/SDL_mixer.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SFX_CHANNEL_COUNT 20

#define SFX_BUTTON4 50300

#define MUS_MAIN 0
#define MUS_VICTORY 1
#define MUS_DEFEAT 2
#define MUS_GAME1 3
#define MUS_GAME2 4
#define MUS_GAME3 5
#define MUS_GAME4 6
#define MUS_GAME5 7
#define MUS_GAME6 8
#define MUS_GAME7 9
#define MUS_GAME8 10
#define MUS_GAME9 11
#define MUS_GAME10 12
/* alternative names */
#define MUS_CHOIRS 5

void sfx_init(void);
void sfx_free(void);

void sfx_play(unsigned id);

void mus_play(unsigned id);

extern volatile int music_playing;
extern unsigned music_index;
extern int in_game;

#ifdef __cplusplus
}
#endif

#endif
