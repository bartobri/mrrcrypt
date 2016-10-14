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

#include "main.h"
#include "modules/keyfile.h"
#include "modules/mirrorfield.h"
#include "modules/visitedmirrors.h"

#define MIRROR_NONE      32
#define MIRROR_FORWARD   47
#define MIRROR_BACKWARD  92
#define MIRROR_STRAIGHT  45
#define DIR_UP           1
#define DIR_DOWN         2
#define DIR_LEFT         3
#define DIR_RIGHT        4

// Function prototypes
void main_shutdown(const char *);

int main(int argc, char *argv[]) {
	int o, r, c, ch, i;
	int autoCreate       = 0;
	char *version        = VERSION;
	char *keyFileName    = DEFAULT_KEY_NAME;
	
	// Run module init functions
	keyfile_init();
	mirrorfield_init();
	visitedmirrors_init();
	
	// Validate supported character count is square
	if (strlen(SUPPORTED_CHARS) % 4 != 0)
		main_shutdown("Invalid character set. Character count must be evenly divisible by 4.");

	// Validate supported character count is compatible with grid width
	if (strlen(SUPPORTED_CHARS) / 4 != GRID_SIZE)
		main_shutdown("Invalid character set. Character count does not match grid width.");

	// Check arguments
	while ((o = getopt(argc, argv, "ak:v")) != -1) {
		switch (o) {
			case 'a':
				autoCreate = 1;
				break;
			case 'k':
				keyFileName = optarg;
				break;
			case 'v':
				printf("mirrorcrypt version %s\n", version);
				return 0;
			case '?':
				if (isprint(optopt))
					fprintf (stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
				main_shutdown("Invalid command option(s).");
		}
	}
	
	// Turn on autoCreate flag for default key file
	if (strcmp(DEFAULT_KEY_NAME, keyFileName) == 0)
		autoCreate = 1;
	
	// Open mirror file
	if (keyfile_open(keyFileName, autoCreate) == 0) {
		if (autoCreate)
			main_shutdown("Could not auto-create key file. Make sure $HOME is set to a writable directory.");
		else
			main_shutdown("Key file not found. Use -a to auto-create.");
	}

	// Read keyfile and build mirror field
	for (i = 0; (ch = keyfile_next_char()) != EOF; ++i)
		if ((mirrorfield_set(i, ch)) == 0)
			break;

	// Close mirror file
	keyfile_close();
	
	// Validate mirror field contents
	if (mirrorfield_validate() == 0)
		main_shutdown("Invalid key file. Invalid content.");

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {

		int direction = 0;
		char ech = 0;

		// If character not supported, just print it and continue the loop
		if (ch == '\0' || strchr(SUPPORTED_CHARS, ch) == NULL) {
			putchar(ch);
			continue;
		}

		// Determining starting row/col and starting direction
		for (r = 0; r < GRID_SIZE; ++r) {
			for (c = 0; c < GRID_SIZE; ++c) {
				if (mirrorfield_get_charup(r, c) == ch)
					direction = DIR_DOWN;
				else if (mirrorfield_get_charleft(r, c) == ch)
					direction = DIR_RIGHT;
				else if (mirrorfield_get_charright(r, c) == ch)
					direction = DIR_LEFT;
				else if (mirrorfield_get_chardown(r, c) == ch)
					direction = DIR_UP;
				
				if (direction)
					break;
			}
			if (direction)
				break;
		}
		
		// Clear visited points
		visitedmirrors_clear();

		// Traverse through the grid
		while (ech == 0) {
			
			// If we hit a mirror that we've already been to, un-rotate it.
			// This is necessary to preserve the ability to reverse the encryption.
			// We can not rotate a mirror more than once per character.
			if (mirrorfield_get_type(r, c) != MIRROR_NONE) {
				if (visitedmirrors_exists(r, c)) {
					if (mirrorfield_get_type(r, c) == MIRROR_FORWARD) {
						mirrorfield_set_type(r, c, MIRROR_BACKWARD);
					} else if (mirrorfield_get_type(r, c) == MIRROR_BACKWARD) {
						mirrorfield_set_type(r, c, MIRROR_STRAIGHT);
					} else if (mirrorfield_get_type(r, c) == MIRROR_STRAIGHT) {
						mirrorfield_set_type(r, c, MIRROR_FORWARD);
					}
				} else {
					visitedmirrors_add(r, c);
				}
			}

			// Forward mirror / Change direction and rotate
			if (mirrorfield_get_type(r, c) == MIRROR_FORWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_LEFT;
				else if (direction == DIR_LEFT)
					direction = DIR_DOWN;
				else if (direction == DIR_RIGHT)
					direction = DIR_UP;
				else if (direction == DIR_UP)
					direction = DIR_RIGHT;
				
				// Spin mirror
				mirrorfield_set_type(r, c, MIRROR_STRAIGHT);
			}
			
			// Straight mirror - Keep same direction, just rotate
			else if (mirrorfield_get_type(r, c) == MIRROR_STRAIGHT) {

				// Spin mirror
				mirrorfield_set_type(r, c, MIRROR_BACKWARD);
			}

			// Backward mirror \ Change direction and rotate
			else if (mirrorfield_get_type(r, c) == MIRROR_BACKWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_RIGHT;
				else if (direction == DIR_LEFT)
					direction = DIR_UP;
				else if (direction == DIR_RIGHT)
					direction = DIR_DOWN;
				else if (direction == DIR_UP)
					direction = DIR_LEFT;
				
				// Spin mirror
				mirrorfield_set_type(r, c, MIRROR_FORWARD);
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
				ech = mirrorfield_get_charup(0, c);
			else if (r >= GRID_SIZE)
				ech = mirrorfield_get_chardown(GRID_SIZE - 1, c);
			else if (c < 0)
				ech = mirrorfield_get_charleft(r, 0);
			else if (c >= GRID_SIZE)
				ech = mirrorfield_get_charright(r, GRID_SIZE - 1);

		}
		putchar(ech);
		
		// Rotate mirrors
		mirrorfield_rotate();
	}

	return 0;
}

void main_shutdown(const char *errmsg) {
	
	// Log a shutdown message
	printf("Shutting down. Reason: %s\n", errmsg);
	
	// Close mirror file (if open)
	keyfile_close();
	
	// Shutdown
	exit(1);
}
