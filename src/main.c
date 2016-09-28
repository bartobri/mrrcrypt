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

#include "modules/mirrorfile.h"
#include "modules/gridpoint.h"

#define MIRROR_NONE      0
#define MIRROR_FORWARD   1
#define MIRROR_BACKWARD  2
#define DIR_UP           1
#define DIR_DOWN         2
#define DIR_LEFT         3
#define DIR_RIGHT        4
#define SUPPORTED_CHARS  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+{}[]|\\:;\"'<>,.?/~\n\t "
#define MIRROR_FILE_NAME "default"
#define MIRROR_FILE_PATH ".config/mirrorcrypt/"

// Function prototypes
void main_shutdown(const char *);

// TODO - Validate mirror file. 
//            Only should have /, \, space, and \n.
//            Should conform to expected grid size

int main(int argc, char *argv[]) {
	int o, w, r, c, ch, i;
	int autoCreateMirrorFile = 0;
	char *supportedChars      = SUPPORTED_CHARS;
	char *mirrorFilePath      = MIRROR_FILE_PATH;
	char *mirrorFileName      = MIRROR_FILE_NAME;
	char *mirrorFilePathName, *mirrorFileFullPathName;
	char *homeDir             = getenv("HOME");
	
	// Run module init functions
	mirrorfile_init();
	gridpoint_init();

	// Check arguments
	while ((o = getopt(argc, argv, "am:")) != -1) {
		switch (o) {
			case 'a':
				autoCreateMirrorFile = 1;
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

	// Combine mirror file path and name in to one string.
	mirrorFilePathName = malloc(strlen(mirrorFilePath) + strlen(mirrorFileName) + 1);
	sprintf(mirrorFilePathName, "%s%s", mirrorFilePath, mirrorFileName);

	// Validate supported character count is square
	if (strlen(supportedChars) % 4 != 0)
		main_shutdown("Invalid character set. Character count must be evenly divisible by 4");

	// Get required grid size
	w = (strlen(supportedChars) / 4);

	// Die if we don't have a home directory
	if (homeDir == NULL)
		main_shutdown("Unable to read HOME environmental variable.");

	// Build mirror file path
	mirrorFileFullPathName = malloc(strlen(mirrorFilePathName) + strlen(homeDir) + 2);
	sprintf(mirrorFileFullPathName, "%s/%s", homeDir, mirrorFilePathName);
	
	// open mirror file
	while (mirrorfile_open(mirrorFileFullPathName) == 0) {
		if (autoCreateMirrorFile) {
			if (mirrorfile_create(mirrorFileFullPathName, w) == 0) {
				main_shutdown("Could not auto-create mirror file.");
			}
		} else
			main_shutdown("Could not open mirror file. Use -a to auto-create.");
	}
	
	// Free memory
	free(mirrorFilePathName);
	free(mirrorFileFullPathName);

	// Populate Grid
	for (r = 0; r < w; ++r) {
		for (c = 0; c < w; ++c) {

			// Set adjacent characters
			if (r == 0)
				gridpoint_set_charup(r, c, supportedChars[c]);
			if (c == 0)
				gridpoint_set_charleft(r, c, supportedChars[(w*2)+r]);
			if (c == (w - 1))
				gridpoint_set_charright(r, c, supportedChars[w+r]);
			if (r == (w - 1))
				gridpoint_set_chardown(r, c, supportedChars[(w*3)+c]);

			// Set mirror type
			int m = mirrorfile_next_char();
			if (m == EOF)
				continue;
			else if (m == 10)
				--c;
			else if (m == '/')
				gridpoint_set_type(r, c, MIRROR_FORWARD);
			else if (m == '\\')
				gridpoint_set_type(r, c, MIRROR_BACKWARD);

		}
	}

	// Close mirror file
	mirrorfile_close();

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {

		int direction;
		char ech = 0;

		for (i = 0; supportedChars[i] != '\0'; ++i)
			if (supportedChars[i] == ch)
				break;

		// If character not supported, just print it and continue the loop
		if (supportedChars[i] == '\0') {
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

		// Traverse through the grid
		while (ech == 0) {

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
			}

			// Forward mirror \. Change direction
			if (gridpoint_get_type(r, c) == MIRROR_BACKWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_RIGHT;
				else if (direction == DIR_LEFT)
					direction = DIR_UP;
				else if (direction == DIR_RIGHT)
					direction = DIR_DOWN;
				else if (direction == DIR_UP)
					direction = DIR_LEFT;
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
	
	// Shutdown
	exit(1);
}
