#--------------------------------------
#
# midiCE - makefile
# By TIny_Hacker
# Copyright 2023 - 2026
# License: LGPL-3.0
#
#--------------------------------------

NAME = MIDICE
ICON = icon.png
DESCRIPTION = "USB MIDI controller"
COMPRESSED = YES
COMPRESSED_MODE = zx0
ARCHIVED = YES
VERSION = 1.0.1

CFLAGS = -Wall -Wextra -Oz -DVERSION_NO=\"$(VERSION)\"
CXXFLAGS = -Wall -Wextra -Oz  -DVERSION_NO=\"$(VERSION)\"

# ----------------------------

include $(shell cedev-config --makefile)
