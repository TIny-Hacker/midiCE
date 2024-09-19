/**
 * --------------------------------------
 * 
 * midiCE Source Code - main.c
 * By TIny_Hacker
 * Some code by Powerbyte7
 * Copyright 2023 - 2024
 * License: GPL-3.0
 * Last Build: August 3, 2024
 * Version: 0.1.0
 * 
 * --------------------------------------
**/

#include "defines.h"

#include <keypadc.h>
#include <string.h>
#include <usbdrvce.h>
#include <stdint.h>

#include <graphx.h>

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

static const usb_string_descriptor_t serial = {
    .bLength = sizeof(manufacturer) + 2,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"0",
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
        //.bRefresh = 0,      (Likely unused)
        //.bSynchAddress = 0,
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
    // .idVendor = 0x1963, // IK Multimedia
    // .idProduct = 0x001F, // Unknown? (iRig Keys Mini)
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

    uint16_t pitchbend = DEFAULT_PITCHBEND;
    bool pitchUpdate = false;

    static uint8_t kb_DataBackup[6];

    static const uint8_t notes[6][8] = {
        {0, 0, 0, 0, 0, 0, 72, 73},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {88, 86, 84, 82, 80, 78, 76, 74},
        {89, 87, 85, 83, 81, 79, 77, 75},
        {46 | DRUMPAD, 44 | DRUMPAD, 42 | DRUMPAD, 40 | DRUMPAD, 38 | DRUMPAD, 36 | DRUMPAD, 0, 0},
        {47 | DRUMPAD, 45 | DRUMPAD, 43 | DRUMPAD, 41 | DRUMPAD, 39 | DRUMPAD, 37 | DRUMPAD, 0, 0},
    };

    gfx_Begin();

    if ((error = usb_Init(handleUsbEvent, NULL, &standard, USB_DEFAULT_INIT_FLAGS)) == USB_SUCCESS) {
        while (kb_AnyKey());
        while (!kb_IsDown(kb_KeyClear)) {
            for (uint8_t i = 0; i < 6; i++) { // Back up important keypad registers to compare after scan
                kb_DataBackup[i] = kb_Data[i + 1];
            }

            kb_Scan();
            usb_HandleEvents();

            for (uint8_t i = 0; i < 6; i++) { // Scan for keyboard
                for (uint8_t j = 0; j < 8; j++) {
                    if (bit(kb_Data[i + 1], j) != bit(kb_DataBackup[i], j) && notes[i][j]) { // kb_Data is not zero indexed or something
                        if (bit(kb_Data[i + 1], j)) { // Key was just pressed
                            midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_ON;
                            midiEvent[1] = MIDI_NOTE_ON << 4;
                        } else {
                            midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_OFF;
                            midiEvent[1] = MIDI_NOTE_OFF << 4;
                        }

                        if (bit(notes[i][j], 7)) { // Drumpad
                            midiEvent[1] = midiEvent[1] | MIDI_CHANNEL9;
                        }

                        midiEvent[2] = notes[i][j] & DRUMPAD_MASK;
                        midiEvent[3] = 0x7F; // Velocity? I'm just copying what Powerbyte's keyboard did for now

                        while (USB_SUCCESS != usb_ScheduleInterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), USB_DEVICE_TO_HOST | 1), &midiEvent, 4, NULL, NULL));
                    }
                }
            }

            if (kb_IsDown(kb_KeyLeft) && pitchbend) {
                if (pitchbend > MOD_PITCHBEND) { // Ensure that the pitchbend value stays within range
                    pitchbend -= MOD_PITCHBEND;
                } else {
                    pitchbend = MIN_PITCHBEND;
                }

                pitchUpdate = true;
            } else if (kb_IsDown(kb_KeyRight) && pitchbend < MAX_PITCHBEND) {
                if (pitchbend < MAX_PITCHBEND - MOD_PITCHBEND) {
                    pitchbend += MOD_PITCHBEND;
                } else {
                    pitchbend = MAX_PITCHBEND;
                }

                pitchUpdate = true;
            } else if (pitchbend < DEFAULT_PITCHBEND) { // Correct pitch bend if keys are released
                if (pitchbend + CORRECT_PITCHBEND < DEFAULT_PITCHBEND) {
                    pitchbend += CORRECT_PITCHBEND;
                } else {
                    pitchbend = DEFAULT_PITCHBEND;
                }

                pitchUpdate = true;
            } else if (pitchbend > DEFAULT_PITCHBEND) {
                if (pitchbend - CORRECT_PITCHBEND > DEFAULT_PITCHBEND) {
                    pitchbend -= CORRECT_PITCHBEND;
                } else {
                    pitchbend = DEFAULT_PITCHBEND;
                }

                pitchUpdate = true;
            }

            if (pitchUpdate) {
                midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_PITCHBEND_CHANGE;
                midiEvent[1] = MIDI_PITCHBEND_CHANGE << 4;
                midiEvent[2] = low(pitchbend << 1) >> 1; // Split 16 bit number into two 7 bit numbers, since MIDI needs the most significant bit clear
                midiEvent[3] = high(pitchbend << 1);

                if (USB_SUCCESS == usb_ScheduleInterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), USB_DEVICE_TO_HOST | 1), &midiEvent, 4, NULL, NULL)) {
                    pitchUpdate = false;
                }
            }
        }
    }

    usb_Cleanup();
    gfx_End();

    return 0;
}
