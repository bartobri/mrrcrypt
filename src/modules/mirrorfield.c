// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "main.h"
#include "modules/mirrorfield.h"

#define MIRROR_NONE      32
#define MIRROR_FORWARD   47
#define MIRROR_BACKWARD  92
#define MIRROR_STRAIGHT  45
#define DIR_UP           1
#define DIR_DOWN         2
#define DIR_LEFT         3
#define DIR_RIGHT        4

// Gridpoint Structure
struct gridPoint {
	int mirrorType;
	unsigned char charUp;
	unsigned char charDown;
	unsigned char charLeft;
	unsigned char charRight;
	int visited;
};

// Static Variables
static struct gridPoint grid[GRID_SIZE][GRID_SIZE];

void mirrorfield_init(void) {
	int r, c;

	// Populate Grid
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {

			// Init struct with default values
			grid[r][c].mirrorType = 0;
			grid[r][c].charUp = 0;
			grid[r][c].charDown = 0;
			grid[r][c].charLeft = 0;
			grid[r][c].charRight = 0;
			grid[r][c].visited = 0;
		}
	}
}

int mirrorfield_set(unsigned char ch) {
	static int i = -1;
	int t;

	// Increment our static counter
	++i;
	
	// Set Mirror Char
	if (i < GRID_SIZE * GRID_SIZE) {
		if (ch != '\0' && strchr(SUPPORTED_MIRROR_TYPES, ch)) {
			grid[i / GRID_SIZE][i % GRID_SIZE].mirrorType = ch;
			return 1;
		} else {
			return 0;
		}
	}
	
	// Set Perimeter Char
	if (i - (GRID_SIZE * GRID_SIZE) < GRID_SIZE * 4) {
		t = i - (GRID_SIZE * GRID_SIZE);

		if (t < GRID_SIZE)
			grid[0][t].charUp = ch;
		else if (t < GRID_SIZE * 2)
			grid[t-GRID_SIZE][GRID_SIZE-1].charRight = ch;
		else if (t < GRID_SIZE * 3)
			grid[t-(GRID_SIZE*2)][0].charLeft = ch;
		else if (t < GRID_SIZE * 4)
			grid[GRID_SIZE-1][t-(GRID_SIZE*3)].charDown = ch;

		return 1;
	}
	
	return 0;
}

int mirrorfield_validate(void) {
	int i, i2, r, c;
	unsigned char perimeters[GRID_SIZE * 4];
	
	// init perimeters array
	memset(perimeters, 0, GRID_SIZE * 4);
	
	// init i
	i = 0;

	// Traverse grid
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {

			// Store top chars
			if (r == 0)
				perimeters[i++] = grid[r][c].charUp;

			// Store right chars
			if (c == GRID_SIZE - 1)
				perimeters[i++] = grid[r][c].charRight;

			// Store left chars
			if (c == 0)
				perimeters[i++] = grid[r][c].charLeft;

			// Store bottom chars
			if (r == GRID_SIZE - 1)
				perimeters[i++] = grid[r][c].charDown;

			// Check mirror type
			if (!grid[r][c].mirrorType || !strchr(SUPPORTED_MIRROR_TYPES, grid[r][c].mirrorType))
				return 0;
			
		}
	}
	
	// Check for duplicate perimeter chars
	for (i = 0; i < GRID_SIZE * 4; ++i)
		for (i2 = i+1; i2 < GRID_SIZE * 4; ++i2)
			if (perimeters[i] == perimeters[i2])
				return 0;
	
	return 1;
}

unsigned char mirrorfield_crypt_char(unsigned char ch, int debug) {
	int r, c;
	unsigned char ech;
	int direction = 0;
	
	// For the debug flag
	struct timespec ts;
	ts.tv_sec = debug / 1000;
	ts.tv_nsec = (debug % 1000) * 1000000;
	
	// Determining starting row/col and starting direction
	if (!direction)
		for (r = 0, c = 0; c < GRID_SIZE; ++c)
			if (grid[r][c].charUp == ch) {
				direction = DIR_DOWN;
				break;
			}
	if (!direction)
		for (r = GRID_SIZE-1, c = 0; c < GRID_SIZE; ++c)
			if (grid[r][c].charDown == ch) {
				direction = DIR_UP;
				break;
			}
	if (!direction)
		for (r = 0, c = 0; r < GRID_SIZE && !direction; ++r)
			if (grid[r][c].charLeft == ch) {
				direction = DIR_RIGHT;
				break;
			}
	if (!direction)
		for (r = 0, c = GRID_SIZE-1; r < GRID_SIZE; ++r)
			if (grid[r][c].charRight == ch) {
				direction = DIR_LEFT;
				break;
			}

	// Traverse through the grid
	while (1) {
		
		// Draw mirror field if debug flag is set
		if (debug) {
			mirrorfield_draw(r, c);
			fflush(stdout);
			nanosleep(&ts, NULL);
		}
		
		// If we hit a mirror that we've already been to, un-rotate it.
		// This is necessary to preserve the ability to reverse the encryption.
		// We can not rotate a mirror more than once per character.
		if (grid[r][c].mirrorType != MIRROR_NONE)
			grid[r][c].visited = 1;

		// Change direction if we hit a mirror
		if (grid[r][c].mirrorType == MIRROR_FORWARD) {
			if (direction == DIR_DOWN)
				direction = DIR_LEFT;
			else if (direction == DIR_LEFT)
				direction = DIR_DOWN;
			else if (direction == DIR_RIGHT)
				direction = DIR_UP;
			else if (direction == DIR_UP)
				direction = DIR_RIGHT;
		} else if (grid[r][c].mirrorType == MIRROR_BACKWARD) {
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
		if (r < 0) {
			ech = grid[0][c].charUp;
			break;
		} else if (r >= GRID_SIZE) {
			ech = grid[GRID_SIZE - 1][c].charDown;
			break;
		} else if (c < 0) {
			ech = grid[r][0].charLeft;
			break;
		} else if (c >= GRID_SIZE) {
			ech = grid[r][GRID_SIZE - 1].charRight;
			break;
		}
	}
	
	// Rotate visited mirrors and reset to zero
	for (r = 0; r < GRID_SIZE; ++r)
		for (c = 0; c < GRID_SIZE; ++c)
			if (grid[r][c].visited) {
				if (grid[r][c].mirrorType == MIRROR_BACKWARD)
					grid[r][c].mirrorType = MIRROR_FORWARD;
				else if (grid[r][c].mirrorType == MIRROR_FORWARD)
					grid[r][c].mirrorType = MIRROR_STRAIGHT;
				else if (grid[r][c].mirrorType == MIRROR_STRAIGHT)
					grid[r][c].mirrorType = MIRROR_BACKWARD;

				grid[r][c].visited = 0;
			}
				
	
	// Return crypted char
	return ech;
}

void mirrorfield_roll(void) {
	int r, c;
	char last, next;
	
	// Start with 0,0 char
	last = grid[0][0].charUp;

	// Rotate top chars
	for (c = 1; c < GRID_SIZE; ++c) {
		next = grid[0][c].charUp;
		grid[0][c].charUp = last;
		last = next;
	}
	// Rotate right chars
	for (r = 0; r < GRID_SIZE; ++r) {
		next = grid[r][GRID_SIZE - 1].charRight;
		grid[r][GRID_SIZE - 1].charRight = last;
		last = next;
	}
	// Rotate left chars
	for (r = 0; r < GRID_SIZE; ++r) {
		next = grid[r][0].charLeft;
		grid[r][0].charLeft = last;
		last = next;
	}
	// Rotate bottom chars
	for (c = 0; c < GRID_SIZE; ++c) {
		next = grid[GRID_SIZE - 1][c].charDown;
		grid[GRID_SIZE - 1][c].charDown = last;
		last = next;
	}
	grid[0][0].charUp = last;
}

void mirrorfield_draw(int pos_r, int pos_c) {
	int r, c;
	static int resetCursor = 0;
	
	// Save cursor position if we need to reset it
	// Otherwise, clear screen.
	if (resetCursor)
		printf("\033[s");
	else
		printf("\033[2J");

	// Set cursor position to 0,0
	printf("\033[H");
	
	for (r = -1; r <= GRID_SIZE; ++r) {

		for (c = -1; c <= GRID_SIZE; ++c) {
			
			// Highlight cell if r/c match
			if (r == pos_r && c == pos_c) {
				printf("\x1B[30m"); // foreground black
				printf("\x1B[47m"); // background white
			}
			
			// Print apropriate mirror field character
			if (r == -1 && c == -1)                     // Upper left corner
				printf("%2c", ' ');
			else if (r == -1 && c == GRID_SIZE)         // Upper right corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE && c == -1)         // Lower left corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE && c == GRID_SIZE)  // Lower right corner
				printf("%2c", ' ');
			else if (r == -1)                           // Top chars
				printf("%2x", grid[r+1][c].charUp);
			else if (c == GRID_SIZE)                    // Right chars
				printf("%2x", grid[r][c-1].charRight);
			else if (r == GRID_SIZE)                    // Bottom chars
				printf("%2x", grid[r-1][c].charDown);
			else if (c == -1)                           // Left chars
				printf("%2x", grid[r][c+1].charLeft);
			else if (grid[r][c].mirrorType == MIRROR_FORWARD)
				printf("%2c", '/');
			else if (grid[r][c].mirrorType == MIRROR_BACKWARD)
				printf("%2c", '\\');
			else if (grid[r][c].mirrorType == MIRROR_STRAIGHT)
				printf("%2c", '-');
			else
				printf("%2c", ' ');
			
			// Un-Highlight cell if r/c match
			if (r == pos_r && c == pos_c)
				printf("\x1B[0m");

		}
		printf("\n");
	}
	printf("\n");
	
	// Restore cursor position if we need to
	if (resetCursor)
		printf("\033[u");
	else
		resetCursor = 1;
}


