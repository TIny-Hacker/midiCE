/**
 * --------------------------------------
 * 
 * midiCE Studio Source Code - utils.h
 * By TIny_Hacker
 * Copyright 2023 - 2025
 * License: LGPL-3.0
 * 
 * --------------------------------------
**/

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Prints a string at a given VRAM address.
 * 
 * @param start VRAM address to begin drawing at.
 * @param string Pointer to string.
 */
void asm_utils_PrintString(void *start, char *string);

/**
 * @brief Text color to draw with.
 * 
 */
extern uint8_t textColor;

#ifdef __cplusplus
}
#endif

#endif
