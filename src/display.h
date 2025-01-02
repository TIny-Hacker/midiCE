/**
 * --------------------------------------
 * 
 * midiCE Studio Source Code - display.h
 * By TIny_Hacker
 * Copyright 2023 - 2025
 * License: GPL-3.0
 * 
 * --------------------------------------
**/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <graphx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draws a dial.
 * 
 * @param dial Background sprite for dial.
 * @param value Value to be displayed by needle.
 * @param max Maximum needle value.
 * @param x Top-left x coordinate of the dial.
 * @param y Top-left y coordinate of the dial.
 */
void display_Dial(gfx_sprite_t *dial, uint8_t value, uint8_t max, unsigned int x, uint8_t y);

/**
 * @brief Draws the values associated with each setting.
 * 
 * @param state State of the program.
 */
void display_Settings(state_t state);

/**
 * @brief Draws the values associated with the current status of the program.
 * 
 * @param state State of the program.
 */
void display_Status(state_t state);

/**
 * @brief Draws all background text.
 * 
 */
void display_Text(void);

#ifdef __cplusplus
}
#endif

#endif
