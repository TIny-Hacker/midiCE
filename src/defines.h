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

#ifdef __cplusplus
}
#endif

#endif
