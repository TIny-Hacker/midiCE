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

static const usb_string_descriptor_t *strings[] = { &product_name, &manufacturer };
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
    usb_interface_descriptor_t interface0;
    audio_control_interface_header_t audio_header;
    usb_interface_descriptor_t interface1;
    midistreaming_interface_header_t midi_header;
    midistreaming_in_descriptor_t midi_in[2];
    midistreaming_out_descriptor_t midi_out[2];
    usb_endpoint_descriptor_t endpoint0;
    midistreaming_endpoint_descriptor_t midi_endpoint0;
    usb_endpoint_descriptor_t endpoint1;
    midistreaming_endpoint_descriptor_t midi_endpoint1;
} configuration1 = {
    .configuration = {
        .bLength = sizeof(configuration1.configuration),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(configuration1),
        .bNumInterfaces = 2,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = USB_BUS_POWERED | USB_NO_REMOTE_WAKEUP,
        .bMaxPower = 500 / 2,
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
        .bNumEndpoints = 2,
        .bInterfaceClass = USB_AUDIO_CLASS,
        .bInterfaceSubClass = 0x03, // MIDI Streaming
        .bInterfaceProtocol = 0, // Unused
        .iInterface = 0, // Optional string index
    },
    .midi_header = {
        .bLength = sizeof(configuration1.midi_header),
        .bDescriptorType = 0x24, // audio class interface
        .bDescriptorSubtype = 0x01, // header descriptor
        .version = 0x0100,
        .wTotalLength = sizeof(configuration1.midi_header) + 
                        sizeof(configuration1.midi_in) +
                        sizeof(configuration1.midi_out), // TODO, add together sizes of all MIDI descriptors
    },
    .midi_in = {
        [0] = {
            .bLength = sizeof(configuration1.midi_in[0]),
            .bDescriptorType = 0x24, // audio class interface
            .bDescriptorSubtype = 0x02, // MIDI IN jack descriptor
            .bJackType = 0x01,
            .bJackID = 0x01,
            .iJack = 0x00, // Optional string index
        },
        [1] = {
            .bLength = sizeof(configuration1.midi_in[1]),
            .bDescriptorType = 0x24, // audio class interface
            .bDescriptorSubtype = 0x02, // MIDI IN jack descriptor
            .bJackType = 0x02,
            .bJackID = 0x02,
            .iJack = 0x00, // Optional string index
        },
    },
    .midi_out = {
        [0] = {
            .bLength = sizeof(configuration1.midi_out[0]),
            .bDescriptorType = 0x24, // audio class interface
            .bDescriptorSubtype = 0x03, // MIDI OUT jack descriptor
            .bJackType = 0x01,
            .bJackID = 0x03,
            .bNrInputPins = 0x01,
            .baSourceID = 0x02,
            .BaSourcePin = 0x01,
            .iJack = 0x00, // Optional string index
        },
        [1] = {
            .bLength = sizeof(configuration1.midi_out[1]),
            .bDescriptorType = 0x24, // audio class interface
            .bDescriptorSubtype = 0x03, // MIDI OUT jack descriptor
            .bJackType = 0x02,
            .bJackID = 0x04,
            .bNrInputPins = 0x01,
            .baSourceID = 0x01,
            .BaSourcePin = 0x01,
            .iJack = 0x00, // Optional string index
        },
    },
    .endpoint0 = {
        .bLength = sizeof(configuration1.endpoint0),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_HOST_TO_DEVICE | 1,
        .bmAttributes = USB_BULK_TRANSFER,
        .wMaxPacketSize = 0x20, // 1x 32 bytes
        .bInterval = 0,
        //.bRefresh = 0,      (Likely unused)
        //.bSynchAddress = 0,
    },
    .midi_endpoint0 = {
        .bLength = sizeof(configuration1.midi_endpoint0),
        .bDescriptorType = 0x25, // Audio class endpoint
        .bDescriptorSubtype = 0x01, // General Descriptor
        .bNumEmbMIDIJack = 1,
        .baAssocJackID = 3,
    },
    .endpoint1 = {
        .bLength = sizeof(configuration1.endpoint1),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_DEVICE_TO_HOST | 1,
        .bmAttributes = USB_BULK_TRANSFER,
        .wMaxPacketSize = 0x20, // 1x 32 bytes
        .bInterval = 0,
        // .bRefresh = 0,      (Likely unused)
        // .bSynchAddress = 0,
    },
    .midi_endpoint1 = {
        .bLength = sizeof(configuration1.midi_endpoint1),
        .bDescriptorType = 0x25, // Audio class endpoint
        .bDescriptorSubtype = 0x01, // General Descriptor
        .bNumEmbMIDIJack = 1,
        .baAssocJackID = 1,
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
    .bcdDevice = 0x0101,
    .iManufacturer = 2,
    .iProduct = 1,
    .iSerialNumber = 0,
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
    bool note = false;
    static uint8_t midiEvent[4];

    gfx_Begin();

    if ((error = usb_Init(handleUsbEvent, NULL, &standard, USB_DEFAULT_INIT_FLAGS)) == USB_SUCCESS) {
        while (kb_AnyKey());
        while (!kb_IsDown(kb_KeyClear)) {
            kb_Scan();
            usb_HandleEvents();

            if (!kb_AnyKey() && note) {
                midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_OFF;
                midiEvent[1] = MIDI_NOTE_OFF << 4;
                midiEvent[2] = 69; // A4
                midiEvent[3] = 0x7F; // Velocity? I'm just copying what Powerbyte's keyboard did for now

                if (USB_SUCCESS == usb_ScheduleInterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), 0x81), &midiEvent, 4, NULL, NULL)) {
                    note = false;
                }
            } else if (kb_AnyKey() && !note) {
                gfx_ZeroScreen();
                midiEvent[0] = MIDI_CABLE0 << 4 | MIDI_NOTE_ON;
                midiEvent[1] = MIDI_NOTE_ON << 4;
                midiEvent[2] = 69;
                midiEvent[3] = 0x7F;

                if (USB_SUCCESS == usb_ScheduleInterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), 0x81), &midiEvent, 4, NULL, NULL)) {
                    note = true;
                }
            }
        }
    }

    usb_Cleanup();
    gfx_End();

    return 0;
}
