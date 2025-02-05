#--------------------------------------
#
# midiCE - makefile
# By TIny_Hacker
# Copyright 2023 - 2025
# License: LGPL-3.0
#
#--------------------------------------

NAME = MIDICE
ICON = icon.png
DESCRIPTION = "USB MIDI controller"
COMPRESSED = YES
ARCHIVED = YES
VERSION = 1.0.1

CFLAGS = -Wall -Wextra -Oz -DVERSION_NO=\"$(VERSION)\"
CXXFLAGS = -Wall -Wextra -Oz  -DVERSION_NO=\"$(VERSION)\"

# ----------------------------

include $(shell cedev-config --makefile)

final:
	make
	convbin -r -e zx0 -k 8xp-compressed -u -n $(NAME) -i bin/$(NAME).bin -o bin/$(NAME).8xp
