/**
 * --------------------------------------
 * 
 * midiCE Source Code - main.c
 * By TIny_Hacker
 * Some code by Powerbyte7
 * Copyright 2023 - 2025
 * License: LGPL-3.0
 * Last Build: January 7, 2025
 * Version: 1.0.1
 * 
 * --------------------------------------
**/

#include "defines.h"
#include "display.h"
#include "settings.h"

#include "gfx/gfx.h"

#include <keypadc.h>
#include <string.h>
#include <usbdrvce.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <ti/info.h>

static const usb_string_descriptor_t product_name = {
    .bLength = sizeof(product_name) + 12, // 12 includes length of .bString
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"midiCE",
};

static const usb_string_descriptor_t manufacturer = {
    .bLength = sizeof(manufacturer) + 22,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"TIny_Hacker",
};

static usb_string_descriptor_t serial = {
    .bLength = sizeof(serial) + 32,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"0000000000000000",
};

static const usb_string_descriptor_t jack = {
    .bLength = sizeof(manufacturer) + 12,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"Output",
};

static const usb_string_descriptor_t *strings[] = { &product_name, &manufacturer, &serial, &jack };
static const usb_string_descriptor_t langids = {
    // .bLength = sizeof(langids),
    .bLength = 4,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = {
        [0] = DEFAULT_LANGID,
    },
};

static struct {
    usb_configuration_descriptor_t configuration;
    usb_interface_association_descriptor_t interface_association;
    usb_interface_descriptor_t interface0;
    audio_control_interface_header_t audio_header;
    usb_interface_descriptor_t interface1;
    midistreaming_interface_header_t midi_header;
    midistreaming_in_descriptor_t midi_in;
    midistreaming_out_descriptor_t midi_out;
    usb_endpoint_descriptor_t endpoint;
    midistreaming_endpoint_descriptor_t midi_endpoint;
} configuration1 = {
    .configuration = {
        .bLength = sizeof(configuration1.configuration),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(configuration1),
        .bNumInterfaces = 2,
        .bConfigurationValue = 1,
        .iConfiguration = 1,
        .bmAttributes = USB_BUS_POWERED | USB_NO_REMOTE_WAKEUP,
        .bMaxPower = 500 / 2,
    },
    .interface_association = {
        .bLength = sizeof(configuration1.interface_association),
        .bDescriptorType = 0x0B,
        .bFirstInterface = 0,
        .bInterfaceCount = 2,
        .bFunctionClass = 0x01,
        .bFunctionSubClass = 0x03,
        .bFunctionProtocol = 0,
        .iFunction = 0,
    },
    .interface0 = {
        .bLength = sizeof(configuration1.interface0),
        .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = USB_AUDIO_CLASS,
        .bInterfaceSubClass = 0x01, // Control Device
        .bInterfaceProtocol = 0, // Unused
        .iInterface = 0, // Optional string index
    },
    .audio_header = {
        .bLength = sizeof(configuration1.audio_header),
        .bDescriptorType = 0x24, // audio class interface
        .bDescriptorSubtype = 0x01, // header descriptor
        .version = 0x0100,
        .wTotalLength = sizeof(configuration1.audio_header), // TODO
        .interfaces = 0x01,
        .interfaceNumber = 0x01,
    },
    .interface1 = {
        .bLength = sizeof(configuration1.interface1),
        .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
        .bInterfaceNumber = 1,
        .bAlternateSetting = 0,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_AUDIO_CLASS,
        .bInterfaceSubClass = 0x03, // MIDI Streaming
        .bInterfaceProtocol = 0, // Unused
        .iInterface = 2, // Optional string index
    },
    .midi_header = {
        .bLength = sizeof(configuration1.midi_header),
        .bDescriptorType = 0x24, // audio class interface
        .bDescriptorSubtype = 0x01, // header descriptor
        .version = 0x0100,
        .wTotalLength = sizeof(configuration1.midi_header) + 
                        sizeof(configuration1.midi_in) +
                        sizeof(configuration1.midi_out),
    },
    .midi_in = {
        .bLength = sizeof(configuration1.midi_in),
        .bDescriptorType = 0x24, // audio class interface
        .bDescriptorSubtype = 0x02, // MIDI IN jack descriptor
        .bJackType = 0x02,
        .bJackID = 0x02,
        .iJack = 0x04, // Optional string index
    },
    .midi_out = {
        .bLength = sizeof(configuration1.midi_out),
        .bDescriptorType = 0x24, // audio class interface
        .bDescriptorSubtype = 0x03, // MIDI OUT jack descriptor
        .bJackType = 0x01,
        .bJackID = 0x12,
        .bNrInputPins = 0x01,
        .baSourceID = 0x12,
        .baSourcePin = 0x01,
        .iJack = 0x04, // Optional string index
    },
    .endpoint = {
        .bLength = sizeof(configuration1.endpoint),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_DEVICE_TO_HOST | 1,
        .bmAttributes = USB_BULK_TRANSFER,
        .wMaxPacketSize = 0x40,
        .bInterval = 0,
    },
    .midi_endpoint = {
        .bLength = sizeof(configuration1.midi_endpoint),
        .bDescriptorType = 0x25, // Audio class endpoint
        .bDescriptorSubtype = 0x01, // General Descriptor
        .bNumEmbMIDIJack = 1,
        .baAssocJackID = 0x12, // midi_out jack
    },
};

static const usb_configuration_descriptor_t *configurations[] = {
    &configuration1.configuration,
};

static const usb_device_descriptor_t device = {
    .bLength = sizeof(device),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 0x40,
    .idVendor = 0x0451, // Texas Instruments
    .idProduct = 0x1337,
    .bcdDevice = 0x0100,
    .iManufacturer = 2,
    .iProduct = 1,
    .iSerialNumber = 3,
    .bNumConfigurations = sizeof(configurations) / sizeof(*configurations),
};

static const usb_standard_descriptors_t standard = {
    .device = &device,
    .configurations = configurations,
    .langids = &langids,
    .numStrings = sizeof(strings) / sizeof(*strings),
    .strings = strings,
};

static usb_error_t handleUsbEvent(usb_event_t event, void *eventData, usb_callback_data_t *callbackData) {
    usb_error_t error = USB_SUCCESS;

    if (event == USB_DEFAULT_SETUP_EVENT) {
        if (((usb_control_setup_t *)eventData)->bRequest == USB_GET_DESCRIPTOR_REQUEST &&
            ((usb_control_setup_t *)eventData)->wValue == 0x0200 &&
            ((usb_control_setup_t *)eventData)->wLength == sizeof(configuration1)) {

            usb_Transfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), 0), &configuration1, sizeof(configuration1), 5, NULL);
            error = USB_IGNORE;
        }
    }

    return error;
}

int main(void) {
    usb_error_t error;
    static uint8_t midiEvent[4];

    state_t state = {
        0,
        MOD_PITCHBEND,
        DEFAULT_VELOCITY,
        VOICE_POLY,
        MIDI_CHANNEL0,
        THEME_DARK,
        0,
        DEFAULT_PITCHBEND,
    };

    bool pitchUpdate = false;

    bool octaveUpPressed = false;
    bool octaveDownPressed = false;

    static uint8_t kbBackup[6] = {0, 0, 0, 0, 0, 0};

    static const uint8_t notes[6][8] = {
        {0, 0, 0, 0, 0, 1 | CONTROL_DOWN, 72, 73},
        {0, 4, 4 | CONTROL_DOWN, 3, 3 | CONTROL_DOWN, 2, 2 | CONTROL_DOWN, 1},
        {88, 86, 84, 82, 80, 78, 76, 74},
        {89, 87, 85, 83, 81, 79, 77, 75},
        {46 | DRUMPAD, 44 | DRUMPAD, 42 | DRUMPAD, 40 | DRUMPAD, 38 | DRUMPAD, 36 | DRUMPAD, 0, 0},
        {47 | DRUMPAD, 45 | DRUMPAD, 43 | DRUMPAD, 41 | DRUMPAD, 39 | DRUMPAD, 37 | DRUMPAD, 0, 0},
    };

    uint8_t monoNote = 0;

    static uint16_t controllers[4] = {DEFAULT_CONTROL, DEFAULT_CONTROL, DEFAULT_CONTROL, DEFAULT_CONTROL};

    const system_info_t *system = os_GetSystemInfo();

    for (uint8_t i = 0; i < 16; i++) {
        serial.bString[i] = 'A' + (system->calcid[i] >> 4);
        i++;
        serial.bString[i] = 'A' + (system->calcid[i] & 0xF);
    };

    gfx_Begin();
    gfx_SetPalette(darkPalette, sizeof_darkPalette, 0);
    gfx_SetTransparentColor(255);
    gfx_SetDrawBuffer();

    display_Text();
    display_Status(state);
    display_Settings(state);

    gfx_SetColor(COLOR_BLUE);
    display_Dial(dial1, state.root, 11, 254, 35);
    gfx_SetColor(COLOR_TAN);
    display_Dial(dial2, state.bend, 0xFF, 254, 105);
    gfx_SetColor(COLOR_ORANGE);
    display_Dial(dial3, state.velocity, MAX_VELOCITY, 254, 174);

    gfx_BlitBuffer();

    clock_t clockOffset = clock();

    if ((error = usb_Init(handleUsbEvent, NULL, &standard, USB_DEFAULT_INIT_FLAGS)) == USB_SUCCESS) {
        while (kb_AnyKey());
        while (!(kb_IsDown(kb_KeyClear) && !kb_Data[1] && !kb_Data[2] && !kb_Data[3] && !kb_Data[4] && !kb_Data[5])) {
            kb_Scan();
            usb_HandleEvents();

            if (kb_IsDown(kb_KeyZoom)) {
                settings_Open(&state);
            }

            for (uint8_t i = 0; i < 6; i++) { // Scan for keyboard
                for (uint8_t j = 0; j < 8; j++) {
                    if (bit(kb_Data[i + 1], j) && notes[i][j] && notes[i][j] <= (4 | CONTROL_DOWN)) {
                        midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_CONTROL_CHANGE;
                        midiEvent[1] = MIDI_CONTROL_CHANGE << 4 | state.channel;

                        if (bit(notes[i][j], 3)) {
                            if (controllers[notes[i][j] & CONTROL_MASK] > CONTROL_CHANGE) {
                                controllers[notes[i][j] & CONTROL_MASK] -= CONTROL_CHANGE;
                            } else {
                                controllers[notes[i][j] & CONTROL_MASK] = MIN_CONTROL;
                            }
                        } else {
                            if (controllers[notes[i][j] & CONTROL_MASK] < MAX_CONTROL - CONTROL_CHANGE) {
                                controllers[notes[i][j] & CONTROL_MASK] += CONTROL_CHANGE;
                            } else {
                                controllers[notes[i][j] & CONTROL_MASK] = MAX_CONTROL;
                            }
                        }

                        midiEvent[2] = CC16 - 1 + notes[i][j] & CONTROL_MASK;
                        midiEvent[3] = high(controllers[notes[i][j] & CONTROL_MASK] << 1);
                        usb_InterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), USB_DEVICE_TO_HOST | 1), &midiEvent, 4, 5, NULL);
                    } else if (bit(kb_Data[i + 1], j) != bit(kbBackup[i], j) && notes[i][j]) { // kb_Data is not zero indexed bc the first two bytes are empty
                        if (bit(kb_Data[i + 1], j)) { // Key was just pressed
                            if (state.voice == VOICE_MONO && monoNote) {
                                midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_OFF;
                                midiEvent[1] = MIDI_NOTE_OFF << 4 | state.channel;
                                midiEvent[2] = monoNote;
                                kbBackup[i] = toggle(kbBackup[i], j); // This key is still held, so make sure it'll get flipped back again
                                monoNote = 0;
                            } else {
                                midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_ON;
                                midiEvent[1] = MIDI_NOTE_ON << 4 | state.channel;
                                midiEvent[2] = notes[i][j];
                                monoNote = notes[i][j];
                            }
                        } else if (notes[i][j]) {
                            if (kb_IsDown(kb_KeyVars)) {
                                continue;
                            }

                            if (state.voice == VOICE_MONO && monoNote != notes[i][j]) {
                                kbBackup[i] = toggle(kbBackup[i], j); // Key is still released, we just don't need to send an off event
                                continue;
                            }

                            midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_OFF;
                            midiEvent[1] = MIDI_NOTE_OFF << 4 | state.channel;
                            midiEvent[2] = notes[i][j];
                            monoNote = 0;
                        }

                        if (bit(midiEvent[2], 7)) { // Drumpad
                            midiEvent[1] = (midiEvent[1] & 0xF0) | MIDI_CHANNEL9;
                            midiEvent[2] &= DRUMPAD_MASK;
                        } else {
                            midiEvent[2] += state.octave + state.root; // Only octave / root shift notes, not drums.
                        }

                        midiEvent[3] = state.velocity;

                        if (USB_SUCCESS == usb_InterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), USB_DEVICE_TO_HOST | 1), &midiEvent, 4, 5, NULL)) {
                            kbBackup[i] = toggle(kbBackup[i], j); // Back up state of key once it has been accounted for
                        }
                    }
                }
            }

            if (kb_IsDown(kb_KeyLeft) && state.pitchbend) {
                if (state.pitchbend > state.bend) { // Ensure that the pitchbend value stays within range
                    state.pitchbend -= state.bend;
                } else {
                    state.pitchbend = MIN_PITCHBEND;
                }

                pitchUpdate = true;
            } else if (kb_IsDown(kb_KeyRight) && state.pitchbend < MAX_PITCHBEND) {
                if (state.pitchbend < MAX_PITCHBEND - state.bend) {
                    state.pitchbend += state.bend;
                } else {
                    state.pitchbend = MAX_PITCHBEND;
                }

                pitchUpdate = true;
            } else if (state.pitchbend < DEFAULT_PITCHBEND) { // Correct pitch bend if keys are released
                if (state.pitchbend + CORRECT_PITCHBEND < DEFAULT_PITCHBEND) {
                    state.pitchbend += CORRECT_PITCHBEND;
                } else {
                    state.pitchbend = DEFAULT_PITCHBEND;
                }

                pitchUpdate = true;
            } else if (state.pitchbend > DEFAULT_PITCHBEND) {
                if (state.pitchbend - CORRECT_PITCHBEND > DEFAULT_PITCHBEND) {
                    state.pitchbend -= CORRECT_PITCHBEND;
                } else {
                    state.pitchbend = DEFAULT_PITCHBEND;
                }

                pitchUpdate = true;
            }

            if (!kb_Data[1] && !kb_Data[3] && !kb_Data[3] && !kb_Data[4] && !kb_Data[5] && !kb_Data[6]) {
                if (kb_IsDown(kb_KeyUp)) {
                    if (!octaveUpPressed && state.octave > OCTAVE_MIN) {
                        state.octave -= 12;
                        octaveUpPressed = true;
                    }
                } else {
                    octaveUpPressed = false;
                }

                if (kb_IsDown(kb_KeyDown)) {
                    if (!octaveDownPressed && state.octave < OCTAVE_MAX) {
                        state.octave += 12;
                        octaveDownPressed = true;
                    }
                } else {
                    octaveDownPressed = false;
                }
            }

            if (pitchUpdate) {
                pitchUpdate = false;
                midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_PITCHBEND_CHANGE;
                midiEvent[1] = MIDI_PITCHBEND_CHANGE << 4 | state.channel;
                midiEvent[2] = low(state.pitchbend << 1) >> 1; // Split 16 bit number into two 7 bit numbers, since MIDI needs the most significant bit clear
                midiEvent[3] = high(state.pitchbend << 1);

                while (!(kb_IsDown(kb_KeyClear) && !kb_Data[1] && !kb_Data[2] && !kb_Data[3] && !kb_Data[4] && !kb_Data[5]) && USB_SUCCESS != usb_ScheduleInterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), USB_DEVICE_TO_HOST | 1), &midiEvent, 4, NULL, NULL)) {
                    kb_Scan();
                }
            }

            if (clock() - clockOffset > CLOCKS_PER_SEC / 20) {
                clockOffset = clock();
                display_Status(state);
                gfx_BlitBuffer();
            }
        }
    }

    usb_Cleanup();
    gfx_End();

    return 0;
}
