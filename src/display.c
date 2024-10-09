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
#include <keypadc.h>

static void display_Rect(unsigned int x, uint8_t y, uint8_t width, uint8_t height) {
    gfx_FillRectangle_NoClip(x + 1, y, width - 2, height);
    gfx_FillRectangle_NoClip(x, y + 1, width, height - 2);
}

void display_Dial(gfx_sprite_t *dial, uint8_t angle, unsigned int x, uint8_t y) {
    gfx_Sprite_NoClip(spokes, x - 2, y - 9);
    gfx_TransparentSprite_NoClip(dial, x, y);
    gfx_RotatedTransparentSprite_NoClip(dialNeedle, x, y, angle);
    display_Rect(x - 21, y + 26, 11, 19);
}

void display_Settings(state_t state) {
    gfx_SetColor(COLOR_TAN);
    display_Rect(197, 80, 11, 35);
    gfx_SetColor(COLOR_BLUE);
    display_Rect(161, 88, 11, 19);
    gfx_SetColor(COLOR_ORANGE);
    display_Rect(125, 80, 11, 35);

    *(&textColor) = COLOR_BACKGROUND;

    if (state.voice == VOICE_MONO) {
        asm_utils_PrintString(coordVRAMBuf(198, 82), "MONO");
    } else {
        asm_utils_PrintString(coordVRAMBuf(198, 82), "POLY");
    }

    static char channel[3] = {'0', '0', '\0'};


    if (state.channel > MIDI_CHANNEL9) {
        channel[0] = '1';
        channel[1] = state.channel - 10 + '0';
    } else {
        channel[1] = state.channel + '0';
    }

    asm_utils_PrintString(coordVRAMBuf(162, 90), channel);

    if (state.theme == THEME_DARK) {
        asm_utils_PrintString(coordVRAMBuf(126, 82), "DARK");
    } else {
        asm_utils_PrintString(coordVRAMBuf(126, 82), "LITE");
    }
}

void display_Status(state_t state) {
    gfx_SetColor(COLOR_LIGHT_GRAY);
    display_Rect(87, 192, 11, 11); // Sustain background
    gfx_FillRectangle_NoClip(26, 21, 25, 198); // Pitchbend bar

    if (kb_IsDown(kb_KeyVars)) {
        gfx_SetColor(COLOR_MAGENTA);
        display_Rect(88, 193, 9, 9);
    }

    gfx_SetColor(COLOR_MED_LT_GRAY);
    gfx_FillRectangle_NoClip(26, 21, 25, (float)198 / (float)(MAX_PITCHBEND + 1) * (float)state.pitchbend);

    gfx_SetColor(COLOR_BACKGROUND);
    gfx_SetPixel(26, 21);
    gfx_SetPixel(50, 21);
    gfx_SetPixel(26, 218);
    gfx_SetPixel(50, 218);
    gfx_FillRectangle_NoClip(89, 98, 7, 8);

    *(&textColor) = COLOR_MED_LT_GRAY;
    static char octave[2] = {0, 0};
    octave[0] = ((state.octave - OCTAVE_MIN) / 12) + '0';
    asm_utils_PrintString(coordVRAMBuf(88, 98), octave);
}

void display_Text(void) {
    gfx_ZeroScreen();

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
