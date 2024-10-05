;--------------------------------------
;
; midiCE Source Code - utils.asm
; By TIny_Hacker
; Copyright 2023 - 2024
; License: GPL-3.0
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
    ld bc, 0

.getChar:
    ld c, (hl)
    xor a, a
    or a, c
    ret z
    push hl
    ld a, c
    sub a, '+'
    ld c, a
    ld b, 9
    mlt bc
    ld hl, _fontData
    add hl, bc
    ld c, (hl)
    ld a, (_textColor)

.putChar:
    inc hl
    push hl ; LUT
    push de
    ld e, (hl)
    pop hl ; VRAM
    push hl
    ld d, 9

.loop:
    sla e
    jr nc, .skipLoad
    ld (hl), a

.skipLoad:
    inc hl
    dec d
    jr nz, .loop
    pop hl
    ld de, 320
    add hl, de
    ex de, hl
    pop hl
    dec c
    jr nz, .putChar
    pop hl
    inc hl
    jr .getChar

_textColor:
    db $FF

include 'font.asm'
