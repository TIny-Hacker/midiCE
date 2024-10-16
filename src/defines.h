/**
 * --------------------------------------
 * 
 * midiCE Studio Source Code - defines.h
 * By TIny_Hacker
 * Some code by Powerbyte7
 * Copyright 2023 - 2024
 * License: GPL-3.0
 * 
 * --------------------------------------
**/

#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Check a specified bit in a number.
 * 
 */
#define bit(n, bit) ((n) & (1U << (bit)))

/**
 * @brief Toggles a specified bit in a number.
 * 
 */
#define toggle(n, bit) (n ^ (1U << bit))

/**
 * @brief Get the low byte of a 16 bit number.
 * 
 */
#define low(n) (n & 0x00FF)

/**
 * @brief Get the high byte of a 16 bit number.
 * 
 */
#define high(n) (n >> 8)

/**
 * @brief Get the VRAM address for a set of coordinates.
 * 
 */
#define coordVRAM(x, y) (0xD40000 + x + y * 320)

/**
 * @brief Get the VRAM address for a set of coordinates in the buffer.
 * 
 */
#define coordVRAMBuf(x, y) (void *)(0xD40000 + x + y * 320 + 320 * 240)

#define DEFAULT_LANGID 0x0409

#define DEFAULT_PITCHBEND   8192    /* Default pitchbend value. */
#define MAX_PITCHBEND       16383   /* Maximum upward pitchbend value. */
#define MIN_PITCHBEND       0       /* Maximum downward pitchbend value. */
#define MOD_PITCHBEND       56      /* Increment or decrement pitchbend value when holding keys. */
#define CORRECT_PITCHBEND   100     /* Increment or decrement pitchbend value to correct it when no keys are pressed. */

#define DRUMPAD             1 << 7  /* Note is on the drumpad. */
#define DRUMPAD_MASK        127     /* Mask drumpad bit out for MIDI data. */

#define CC16                16      /* CC16 MIDI controller. */
#define CC17                17      /* CC17 MIDI controller. */
#define CC18                18      /* CC18 MIDI controller.*/
#define CC19                19      /* CC19 MIDI controller. */

#define DEFAULT_CONTROL     8192    /* Default control value. */
#define MAX_CONTROL         16383   /* Maximum control value. */
#define MIN_CONTROL         0       /* Minimum control value. */
#define CONTROL_CHANGE      32      /* Increment or decrement control value. */
#define CONTROL_DOWN        8       /* Control key decreases value. */
#define CONTROL_MASK        7       /* Mask control up / down out. */

#define OCTAVE_MAX          2 * 12  /* Maximum octave shift value. */
#define OCTAVE_MIN          -5 * 12 /* Minimum octave shift value. */

#define DEFAULT_VELOCITY    0x64    /* Default velocity value for note events. */
#define MAX_VELOCITY        0x7F

#define VOICE_MONO          0       /* Mono voice. */
#define VOICE_POLY          1       /* Poly voice. */

#define THEME_DARK          0       /* Dark theme setting. */
#define THEME_LIGHT         1       /* Light theme setting. */

// Custom struct definitions

typedef struct usb_interface_association_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bFirstInterface;
    uint8_t bInterfaceCount;
    uint8_t bFunctionClass;
    uint8_t bFunctionSubClass;
    uint8_t bFunctionProtocol;
    uint8_t iFunction;
} usb_interface_association_descriptor_t;

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
    uint8_t baSourcePin;
    uint8_t iJack;
} midistreaming_out_descriptor_t;

typedef struct midistreaming_endpoint_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bNumEmbMIDIJack;
    uint8_t baAssocJackID;
} midistreaming_endpoint_descriptor_t;

typedef struct state {
    uint8_t root;
    uint8_t bend;
    uint8_t velocity;
    uint8_t voice;
    uint8_t channel;
    bool theme;
    int8_t octave;
    uint16_t pitchbend;
} state_t;

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
 * @brief MIDI Channel Numbers
 * 
 */
enum {
    MIDI_CHANNEL0,
    MIDI_CHANNEL1,
    MIDI_CHANNEL2,
    MIDI_CHANNEL3,
    MIDI_CHANNEL4,
    MIDI_CHANNEL5,
    MIDI_CHANNEL6,
    MIDI_CHANNEL7,
    MIDI_CHANNEL8,
    MIDI_CHANNEL9,
    MIDI_CHANNEL10,
    MIDI_CHANNEL11,
    MIDI_CHANNEL12,
    MIDI_CHANNEL13,
    MIDI_CHANNEL14,
    MIDI_CHANNEL15
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

/**
 * @brief Common palette color indexes.
 * 
 */
enum {
    COLOR_BACKGROUND,
    COLOR_BLUE,
    COLOR_TAN,
    COLOR_ORANGE,
    COLOR_DARK_GRAY,
    COLOR_MED_GRAY,
    COLOR_MED_LT_GRAY,
    COLOR_LIGHT_GRAY,
    COLOR_MAGENTA
};

#ifdef __cplusplus
}
#endif

#endif
