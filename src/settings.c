/**
 * --------------------------------------
 * 
 * midiCE Studio Source Code - settings.c
 * By TIny_Hacker
 * Copyright 2023 - 2024
 * License: GPL-3.0
 * 
 * --------------------------------------
**/

#include "asm/utils.h"
#include "defines.h"
#include "display.h"
#include "gfx/gfx.h"

#include <keypadc.h>
#include <time.h>

static void settings_UpdateSelect(uint8_t option) {
    switch (option) {
        case 0:
            asm_utils_PrintString(coordVRAMBuf(236, 16), ","); // ▲
            asm_utils_PrintString(coordVRAMBuf(236, 82), "-"); // ▼
            break;
        case 1:
            asm_utils_PrintString(coordVRAMBuf(236, 86), ","); // ▲
            asm_utils_PrintString(coordVRAMBuf(236, 152), "-"); // ▼
            break;
        case 2:
            asm_utils_PrintString(coordVRAMBuf(236, 155), ","); // ▲
            asm_utils_PrintString(coordVRAMBuf(236, 221), "-"); // ▼
            break;
        case 3:
            asm_utils_PrintString(coordVRAMBuf(192, 95), "."); // ◀
            asm_utils_PrintString(coordVRAMBuf(210, 95), "/"); // ▶
            break;
        case 4:
            asm_utils_PrintString(coordVRAMBuf(156, 95), "."); // ◀
            asm_utils_PrintString(coordVRAMBuf(174, 95), "/"); // ▶
            break;
        case 5:
            asm_utils_PrintString(coordVRAMBuf(120, 95), "."); // ◀
            asm_utils_PrintString(coordVRAMBuf(138, 95), "/"); // ▶
            break;
    }

    gfx_BlitBuffer();
}

void settings_Open(state_t *state) {
    while(kb_AnyKey());

    bool keyPressed = false;
    bool modifying = false;
    uint8_t option = 0;

    clock_t clockOffset = clock();

    *(&textColor) = COLOR_MED_GRAY;
    settings_UpdateSelect(option);

    while (!kb_IsDown(kb_KeyClear) && !kb_IsDown(kb_KeyZoom)) {
        kb_Scan();

        if (!kb_Data[7] && keyPressed) {
            keyPressed = false;
            clockOffset = clock();
        }

        if (kb_IsDown(kb_Key2nd)) {
            modifying = !modifying;
            while (kb_AnyKey());
        }

        if (kb_Data[7] && (!keyPressed || clock() - clockOffset > CLOCKS_PER_SEC / 12)) {
            clockOffset = clock();

            if (!modifying) {
                *(&textColor) = COLOR_BACKGROUND;
                settings_UpdateSelect(option);

                if (kb_IsDown(kb_KeyRight)) {
                    if (option < 3) {
                        option = 5;
                    } else if (option == 3) {
                        option = 0;
                    } else {
                        option--;
                    }
                } else if (kb_IsDown(kb_KeyLeft)) {
                    if (option < 3) {
                        option = 3;
                    } else if (option == 5) {
                        option = 0;
                    } else {
                        option++;
                    }
                }

                if (kb_IsDown(kb_KeyUp)) {
                    if (option) {
                        option--;
                    } else {
                        option = 5;
                    }
                } else if (kb_IsDown(kb_KeyDown)) {
                    if (option < 5) {
                        option++;
                    } else {
                        option = 0;
                    }
                }

                *(&textColor) = COLOR_MED_GRAY;
                settings_UpdateSelect(option);
            } else {
                switch (option) {
                    case 0:
                        if (kb_IsDown(kb_KeyUp)) {
                            if (state->root) {
                                state->root -= 1;
                            } else {
                                state->root = 11;
                            }
                        } else if (kb_IsDown(kb_KeyDown)) {
                            if (state->root < 11) {
                                state->root += 1;
                            } else {
                                state->root = 0;
                            }
                        }

                        gfx_SetColor(COLOR_BLUE);
                        display_Dial(dial1, state->root, 11, 254, 35);
                        break;
                    case 1:
                        if (kb_IsDown(kb_KeyUp)) {
                            state->bend -= 1;
                        } else if (kb_IsDown(kb_KeyDown)) {
                            state->bend += 1;
                        }

                        gfx_SetColor(COLOR_TAN);
                        display_Dial(dial2, state->bend, 0xFF, 254, 105);
                        break;
                    case 2:
                        if (kb_IsDown(kb_KeyUp)) {
                            state->velocity -= 1;
                        } else if (kb_IsDown(kb_KeyDown)) {
                            state->velocity += 1;
                        }

                        gfx_SetColor(COLOR_ORANGE);
                        display_Dial(dial3, state->velocity, 0xFF, 254, 174);
                        break;
                    case 3:
                        if (kb_IsDown(kb_KeyLeft) || kb_IsDown(kb_KeyRight)) {
                            state->voice = !state->voice;
                        }

                        display_Settings(*state);
                        break;
                    case 4:
                        if (kb_IsDown(kb_KeyLeft)) {
                            if (state->channel) {
                                state->channel -= 1;
                            } else {
                                state->channel = MIDI_CHANNEL15;
                            }
                        } else if (kb_IsDown(kb_KeyRight)) {
                            if (state->channel < MIDI_CHANNEL15) {
                                state->channel += 1;
                            } else {
                                state->channel = MIDI_CHANNEL0;
                            }
                        }

                        display_Settings(*state);
                        break;
                    case 5:
                        if (kb_IsDown(kb_KeyLeft) || kb_IsDown(kb_KeyRight)) {
                            state->theme = !state->theme;
                        }

                        display_Settings(*state);
                        break;
                }

                gfx_BlitBuffer();
            }

            if (!keyPressed) {
                while (clock() - clockOffset < CLOCKS_PER_SEC / 2.5 && kb_Data[7]) {
                    kb_Scan();
                }
            }

            keyPressed = true;
        }
    }

    if (kb_IsDown(kb_KeyZoom)) {
        *(&textColor) = COLOR_BACKGROUND;
        settings_UpdateSelect(option);
        while(kb_AnyKey());
    }
}
