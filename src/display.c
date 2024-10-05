/**
 * --------------------------------------
 * 
 * midiCE Studio Source Code - display.c
 * By TIny_Hacker
 * Copyright 2023 - 2024
 * License: GPL-3.0
 * 
 * --------------------------------------
**/

#include "asm/utils.h"
#include "defines.h"
#include "gfx/gfx.h"

#include <graphx.h>

void display_Update(void) {
    gfx_ZeroScreen();

    gfx_Sprite_NoClip(spokes, 252, 26);
    gfx_Sprite_NoClip(spokes, 252, 96);
    gfx_Sprite_NoClip(spokes, 252, 165);

    gfx_Sprite_NoClip(dial1, 254, 35);
    gfx_Sprite_NoClip(dial2, 254, 105);
    gfx_Sprite_NoClip(dial3, 254, 174);

    // Draw rest of dials here - all of this should go into a function next

    *(&textColor) = COLOR_MED_LT_GRAY;
    asm_utils_PrintString(coordVRAMBuf(234, 22), "ROOT");
    asm_utils_PrintString(coordVRAMBuf(234, 92), "BEND");
    asm_utils_PrintString(coordVRAMBuf(234, 161), "VEL.");
    asm_utils_PrintString(coordVRAMBuf(88, 21), "OCTAVE");
    asm_utils_PrintString(coordVRAMBuf(88, 121), "SUSTAIN");

    *(&textColor) = COLOR_TAN;
    asm_utils_PrintString(coordVRAMBuf(198, 21), "VOICE");

    *(&textColor) = COLOR_BLUE;
    asm_utils_PrintString(coordVRAMBuf(162, 21), "CHANNEL");

    *(&textColor) = COLOR_ORANGE;
    asm_utils_PrintString(coordVRAMBuf(126, 21), "THEME");

    *(&textColor) = COLOR_LIGHT_GRAY;
    asm_utils_PrintString(coordVRAMBuf(53, 21), "PITCHBEND");
}
