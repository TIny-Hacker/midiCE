/**
 * --------------------------------------
 * 
 * midiCE Studio Source Code - defines.h
 * By TIny_Hacker
 * Some code by Powerbyte7
 * Copyright 2023
 * License: GPL-3.0
 * 
 * --------------------------------------
**/

#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_LANGID 0x0409

// Custom struct definitions

typedef struct audio_control_interface_header {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint16_t version; // MIDI version
    uint16_t wTotalLength;
    uint8_t  interfaces;
    uint8_t  interfaceNumber;
} audio_control_interface_header_t;

typedef struct midistreaming_interface_header {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint16_t version; // MIDI version
    uint16_t wTotalLength;
} midistreaming_interface_header_t;

typedef struct midistreaming_in_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bJackType;
    uint8_t bJackID;
    uint8_t iJack;
} midistreaming_in_descriptor_t;

typedef struct midistreaming_out_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bJackType;
    uint8_t bJackID;
    uint8_t bNrInputPins;
    uint8_t baSourceID;
    uint8_t BaSourcePin;
    uint8_t iJack;
} midistreaming_out_descriptor_t;

typedef struct midistreaming_endpoint_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bNumEmbMIDIJack;
    uint8_t baAssocJackID;
} midistreaming_endpoint_descriptor_t;

/**
 * @brief MIDI Event Cable Numbers
 * 
 */
enum {
    MIDI_CABLE0,
    MIDI_CABLE1,
    MIDI_CABLE2,
    MIDI_CABLE3,
    MIDI_CABLE4,
    MIDI_CABLE5,
    MIDI_CABLE6,
    MIDI_CABLE7,
    MIDI_CABLE8,
    MIDI_CABLE9,
    MIDI_CABLE10,
    MIDI_CABLE11,
    MIDI_CABLE12,
    MIDI_CABLE13,
    MIDI_CABLE14,
    MIDI_CABLE15
};

/**
 * @brief MIDI Event Code Index Numbers
 * 
 */
enum {
    MIDI_MISC_EVENT,
    MIDI_CABLE_EVENT,
    MIDI_SYSTEM_2BYTE,
    MIDI_SYSTEM_3BYTE,
    MIDI_SYSEX_STARTS,
    MIDI_SYSTEM_1BYTE,
    MIDI_SYSEX_ENDS_2BYTE,
    MIDI_SYSEX_ENDS_3BYTE,
    MIDI_NOTE_OFF,
    MIDI_NOTE_ON,
    MIDI_POLY_KEYPRESS,
    MIDI_CONTROL_CHANGE,
    MIDI_PROGRAM_CHANGE,
    MIDI_CHANNEL_PRESSURE,
    MIDI_PITCHBEND_CHANGE,
    MIDI_SINGLE_BYTE
};

#ifdef __cplusplus
}
#endif

#endif
