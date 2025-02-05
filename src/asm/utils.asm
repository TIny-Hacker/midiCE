;--------------------------------------
;
; midiCE Source Code - utils.asm
; By TIny_Hacker
; Copyright 2023 - 2025
; License: LGPL-3.0
;
;--------------------------------------

    assume adl=1

    section .text

    public _asm_utils_PrintString
    public _textColor

include 'include/ti84pceg.inc'

_asm_utils_PrintString:
    pop bc
    pop de ; VRAM pointer
    ex (sp), hl ; String pointer
    push de
    push bc

.getChar:
    ld a, (hl)
    or a, a
    ret z
    push hl
    sub a, '+'
    ld c, a
    ld b, 9
    mlt bc
    ld hl, _fontData
    add hl, bc
    ld c, (hl)

.putChar:
    inc hl
    push hl ; LUT
    push de
    ld a, (hl)
    pop hl ; VRAM
    push hl
    ld d, 9

.loop:
    add a, a
    jr nc, .skipLoad
    ld (hl), $FF
_textColor := $ - 1

.skipLoad:
    inc hl
    dec d
    jr nz, .loop
    pop hl
    ld de, ti.lcdWidth
    add hl, de
    ex de, hl
    pop hl
    dec c
    jr nz, .putChar
    pop hl
    inc hl
    jr .getChar

include 'font.asm'
