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

#define DIR_UP           1
#define DIR_DOWN         2
#define DIR_LEFT         3
#define DIR_RIGHT        4
#define MIRROR_NONE      0
#define MIRROR_FORWARD   1
#define MIRROR_BACKWARD  2
#define SUPPORTED_CHARS  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+{}[]|\\:;\"'<>,.?/~\n\t "
#define MIRROR_FILE_NAME "default"
#define MIRROR_FILE_PATH ".config/mirrorcrypt/"

struct gridPoint {
	int mirrorType;
	char charUp;
	char charDown;
	char charLeft;
	char charRight;
};

// Function prototypes
int create_mirror_file(char *, int);

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
				exit(1);
		}
	}

	// Combine mirror file path and name in to one string.
	mirrorFilePathName = malloc(strlen(mirrorFilePath) + strlen(mirrorFileName) + 1);
	sprintf(mirrorFilePathName, "%s%s", mirrorFilePath, mirrorFileName);

	// Validate supported character count is square
	if (strlen(supportedChars) % 4 != 0) {
		fprintf(stderr, "Invalid character set. Character count must be evenly divisible by 4\n");
		return 1;
	}

	// Get required grid size
	w = (strlen(supportedChars) / 4);

	// Die if we don't have a home directory
	if (homeDir == NULL) {
		fprintf(stderr, "Unable to read HOME environmental variable.\n");
		return 1;
	}

	// Build mirror file path
	mirrorFileFullPathName = malloc(strlen(mirrorFilePathName) + strlen(homeDir) + 2);
	sprintf(mirrorFileFullPathName, "%s/%s", homeDir, mirrorFilePathName);
	
	// open mirror file
	while (mirrorfile_open(mirrorFileFullPathName) == 0) {
		if (autoCreateMirrorFile) {
			if (create_mirror_file(mirrorFileFullPathName, w) == 0) {
				return 1;
			}
		} else
			return 1;
		
		// TODO - create a main_exit() to handle the above returns/exits
	}
	
	// Free memory
	free(mirrorFilePathName);
	free(mirrorFileFullPathName);

	// Populate Grid
	struct gridPoint grid[w][w];
	for (r = 0; r < w; ++r) {
		for (c = 0; c < w; ++c) {

			// Init struct with default values
			grid[r][c].mirrorType = MIRROR_NONE;
			grid[r][c].charUp = 0;
			grid[r][c].charDown = 0;
			grid[r][c].charLeft = 0;
			grid[r][c].charRight = 0;

			// Set adjacent characters
			if (r == 0)
				grid[r][c].charUp = supportedChars[c];
			if (c == 0)
				grid[r][c].charLeft = supportedChars[(w*2)+r];
			if (c == (w - 1))
				grid[r][c].charRight = supportedChars[w+r];
			if (r == (w - 1))
				grid[r][c].charDown = supportedChars[(w*3)+c];

			// Set mirror type
			int m = fgetc(mirrorFile);
			if (m == EOF)
				continue;
			else if (m == 10)
				--c;
			else if (m == '/')
				grid[r][c].mirrorType = MIRROR_FORWARD;
			else if (m == '\\')
				grid[r][c].mirrorType = MIRROR_BACKWARD;

		}
	}

	// Close mirror file
	fclose(mirrorFile);

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
			if (grid[r][c].mirrorType == MIRROR_FORWARD) {
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
			if (grid[r][c].mirrorType == MIRROR_BACKWARD) {
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
				ech = grid[0][c].charUp;
			else if (r >= w)
				ech = grid[w-1][c].charDown;
			else if (c < 0)
				ech = grid[r][0].charLeft;
			else if (c >= w)
				ech = grid[r][w-1].charRight;

		}
		putchar(ech);
	}

	return 0;
}
