// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <ctype.h>
#include <stdbool.h>

#include "modules/supportedchars.h"
#include "modules/mirrorfile.h"
#include "modules/gridpoint.h"
#include "modules/visitedpoints.h"

#define MIRROR_NONE      0
#define MIRROR_FORWARD   1
#define MIRROR_BACKWARD  2
#define DIR_UP           1
#define DIR_DOWN         2
#define DIR_LEFT         3
#define DIR_RIGHT        4

// Function prototypes
void main_shutdown(const char *);

int main(int argc, char *argv[]) {
	int o, w, r, c, ch, i;
	int autoCreate = 0;
	char *homeDir = getenv("HOME");
	char *mirrorFileName = NULL;
	
	// Run module init functions
	supportedchars_init();
	mirrorfile_init();
	gridpoint_init();
	visitedpoints_init();

	// Die if we don't have a home directory
	if (homeDir == NULL)
		main_shutdown("Unable to read HOME environment variable.");
	
	// Validate supported character count is square
	if (supportedchars_count() % 4 != 0)
		main_shutdown("Invalid character set. Character count must be evenly divisible by 4.");

	// Validate supported character count is compatible with grid width
	if (supportedchars_count() / 4 != gridpoint_get_width())
		main_shutdown("Invalid character set. Character count does not match grid width.");

	// Check arguments
	while ((o = getopt(argc, argv, "am:")) != -1) {
		switch (o) {
			case 'a':
				autoCreate = 1;
				break;
			case 'm':
				mirrorFileName = optarg;
				break;
			case '?':
				if (isprint(optopt))
					fprintf (stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
				main_shutdown("Invalid command option(s).");
		}
	}
	
	// open mirror file
	while (mirrorfile_open(homeDir, mirrorFileName) == 0) {
		if (autoCreate) {
			if (mirrorfile_create(homeDir, mirrorFileName, gridpoint_get_width()) == 0) {
				main_shutdown("Could not auto-create mirror file.");
			}
		} else
			main_shutdown("Could not open mirror file. Use -a to auto-create.");
	}
	
	// Get grid width
	w = gridpoint_get_width();

	// Populate Grid
	for (r = 0; r < w; ++r) {
		for (c = 0; c < w; ++c) {

			// Set adjacent characters
			if (r == 0)
				gridpoint_set_charup(r, c, supportedchars_get(c));
			if (c == 0)
				gridpoint_set_charleft(r, c, supportedchars_get((w*2)+r));
			if (c == (w - 1))
				gridpoint_set_charright(r, c, supportedchars_get(w+r));
			if (r == (w - 1))
				gridpoint_set_chardown(r, c, supportedchars_get((w*3)+c));

			// Set mirror type
			ch = mirrorfile_next_char();

			// Shutdown if mirror file is not open
			if (ch == -2)
				main_shutdown("Could not read from mirror file. Mirror file not open.");
			
			// Break out of loop if we reached the end of the mirror file
			if (ch == EOF)
				break;
			
			// Evaluate mirror file character
			if (ch == 10)
				if (c == 0)
					--c;
				else
					main_shutdown("Invalid mirror file. Width does not conform.");
			else if (ch == '/')
				gridpoint_set_type(r, c, MIRROR_FORWARD);
			else if (ch == '\\')
				gridpoint_set_type(r, c, MIRROR_BACKWARD);
			else if (ch == ' ')
				gridpoint_set_type(r, c, MIRROR_NONE);
			else
				main_shutdown("Invalid character in mirror file.");

		}
		if (ch == EOF)
			break;
	}
	
	if (r != w && c != w)
		main_shutdown("Invalid mirror file. Too short.");

	// Close mirror file
	mirrorfile_close();

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {

		int direction;
		char ech = 0;

		for (i = 0; supportedchars_get(i) != '\0'; ++i)
			if (supportedchars_get(i) == ch)
				break;

		// If character not supported, just print it and continue the loop
		if (supportedchars_get(i) == '\0') {
			putchar(ch);
			continue;
		}

		// Determining starting row/col and starting direction
		if (i < w) {
			r = 0;
			c = i;
			direction = DIR_DOWN;
		} else if (i >= w && i < (w*2)) {
			r = i - w;
			c = w - 1;
			direction = DIR_LEFT;
		} else if (i >= (w*2) && i < (w*3)) {
			r = i - (w*2);
			c = 0;
			direction = DIR_RIGHT;
		} else {
			r = w - 1;
			c = i - (w*3);
			direction = DIR_UP;
		}
		
		// Clear visited points
		visitedpoints_clear();

		// Traverse through the grid
		while (ech == 0) {
			
			// If we hit a mirror that we've already been to, un-rotate it.
			// This is necessary to preserve the ability to reverse the encryption.
			// We can not rotate a mirror more than once per character.
			if (gridpoint_get_type(r, c) != MIRROR_NONE) {
				if (visitedpoints_exists(r, c)) {
					if (gridpoint_get_type(r, c) == MIRROR_FORWARD) {
						gridpoint_set_type(r, c, MIRROR_BACKWARD);
					} else if (gridpoint_get_type(r, c) == MIRROR_BACKWARD) {
						gridpoint_set_type(r, c, MIRROR_FORWARD);
					}
				} else {
					visitedpoints_add(r, c);
				}
			}

			// Forward mirror /. Change direction
			if (gridpoint_get_type(r, c) == MIRROR_FORWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_LEFT;
				else if (direction == DIR_LEFT)
					direction = DIR_DOWN;
				else if (direction == DIR_RIGHT)
					direction = DIR_UP;
				else if (direction == DIR_UP)
					direction = DIR_RIGHT;
				
				// Spin mirror
				gridpoint_set_type(r, c, MIRROR_BACKWARD);
			}

			// backward mirror \. Change direction
			else if (gridpoint_get_type(r, c) == MIRROR_BACKWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_RIGHT;
				else if (direction == DIR_LEFT)
					direction = DIR_UP;
				else if (direction == DIR_RIGHT)
					direction = DIR_DOWN;
				else if (direction == DIR_UP)
					direction = DIR_LEFT;
				
				// Spin mirror
				gridpoint_set_type(r, c, MIRROR_FORWARD);
			}

			// Advance position
			if (direction == DIR_DOWN)
				++r;
			else if (direction == DIR_LEFT)
				--c;
			else if (direction == DIR_RIGHT)
				++c;
			else if (direction == DIR_UP)
				--r;

			// Check if our position is out of grid bounds. That means we found our char.
			if (r < 0)
				ech = gridpoint_get_charup(0, c);
			else if (r >= w)
				ech = gridpoint_get_chardown(w-1, c);
			else if (c < 0)
				ech = gridpoint_get_charleft(r, 0);
			else if (c >= w)
				ech = gridpoint_get_charright(r, w-1);

		}
		putchar(ech);
	}

	return 0;
}

void main_shutdown(const char *errmsg) {
	
	// Log a shutdown message
	printf("Shutting down. Reason: %s\n", errmsg);
	
	// Close mirror file (if open)
	mirrorfile_close();
	
	// Shutdown
	exit(1);
}
