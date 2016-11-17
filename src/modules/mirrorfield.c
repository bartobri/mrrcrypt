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
	int visited;
};

// Static Variables
static struct gridPoint grid[GRID_SIZE][GRID_SIZE];
static unsigned char perimeterChars[GRID_SIZE * 8];

void mirrorfield_init(void) {
	int r, c;

	// Init struct with default values
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {
			grid[r][c].mirrorType = 0;
			grid[r][c].visited = 0;
		}
	}
	
	// Set perimeter chars to zero
	memset(perimeterChars, 0, sizeof(perimeterChars));
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
	
	// Set Inner Perimeter Chars
	t = i - (GRID_SIZE * GRID_SIZE);
	if (t < GRID_SIZE * 8) {
		perimeterChars[t] = ch;
		return 1;
	}
	
	return 0;
}

int mirrorfield_validate(void) {
	int i, i2, r, c;

	// Check mirrors
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {

			if (!grid[r][c].mirrorType || !strchr(SUPPORTED_MIRROR_TYPES, grid[r][c].mirrorType))
				return 0;
			
		}
	}
	
	// Check for duplicate perimeter chars
	for (i = 0; i < GRID_SIZE * 8; ++i) {
		for (i2 = i+1; i2 < GRID_SIZE * 8; ++i2) {
			if (perimeterChars[i] == perimeterChars[i2]) {
				return 0;
			}
		}
	}
	
	return 1;
}

unsigned char mirrorfield_crypt_char(unsigned char ch, int debug) {
	int r, c;
	unsigned char ech;
	int direction = 0;
	int isAlt = 0;
	int startCharPos;
	int endCharPos;
	int startRollCharPos;
	int endRollCharPos;
	unsigned char tempChar;
	
	static int lastStartCharPos = -1;
	static int lastEndCharPos = -1;
	
	// For the debug flag
	struct timespec ts;
	ts.tv_sec = debug / 1000;
	ts.tv_nsec = (debug % 1000) * 1000000;
	
	// Determine perimeter char location
	for (startCharPos = 0; startCharPos < GRID_SIZE * 8; ++startCharPos)
		if (perimeterChars[startCharPos] == ch)
			break;

	// Determining starting row/col and starting direction
	if (startCharPos < GRID_SIZE) {
		direction = DIR_DOWN;
		isAlt = 0;
		r = 0;
		c = startCharPos;
	} else if (startCharPos < GRID_SIZE * 2) {
		direction = DIR_LEFT;
		isAlt = 0;
		r = startCharPos - GRID_SIZE;
		c = GRID_SIZE - 1;
	} else if (startCharPos < GRID_SIZE * 3) {
		direction = DIR_RIGHT;
		isAlt = 0;
		r = startCharPos - (GRID_SIZE * 2);
		c = 0;
	} else if (startCharPos < GRID_SIZE * 4) {
		direction = DIR_UP;
		isAlt = 0;
		r = GRID_SIZE - 1;
		c = startCharPos - (GRID_SIZE * 3);
	} else if (startCharPos < GRID_SIZE * 5) {
		direction = DIR_DOWN;
		isAlt = 1;
		r = 0;
		c = startCharPos - (GRID_SIZE * 4);
	} else if (startCharPos < GRID_SIZE * 6) {
		direction = DIR_LEFT;
		isAlt = 1;
		r = startCharPos - (GRID_SIZE * 5);
		c = GRID_SIZE - 1;
	} else if (startCharPos < GRID_SIZE * 7) {
		direction = DIR_RIGHT;
		isAlt = 1;
		r = startCharPos - (GRID_SIZE * 6);
		c = 0;
	} else if (startCharPos < GRID_SIZE * 8) {
		direction = DIR_UP;
		isAlt = 1;
		r = GRID_SIZE - 1;
		c = startCharPos - (GRID_SIZE * 7);
	}

	// Traverse through the grid
	while (1) {
		
		// Draw mirror field if debug flag is set
		if (debug) {
			mirrorfield_draw(r, c);
			fflush(stdout);
			nanosleep(&ts, NULL);
		}
		
		// Flag the mirrors we visited so we can rotate them
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
			if (isAlt)
				c += (GRID_SIZE * 4);

			endCharPos = c;
			ech = perimeterChars[endCharPos];
			break;
		} else if (c >= GRID_SIZE) {
			if (isAlt)
				r += (GRID_SIZE * 4);

			endCharPos = r + GRID_SIZE;
			ech = perimeterChars[endCharPos];
			break;
		} else if (c < 0) {
			if (isAlt)
				r += (GRID_SIZE * 4);

			endCharPos = r + (GRID_SIZE * 2);
			ech = perimeterChars[endCharPos];
			break;
		} else if (r >= GRID_SIZE) {
			if (isAlt)
				c += (GRID_SIZE * 4);

			endCharPos = c + (GRID_SIZE * 3);
			ech = perimeterChars[endCharPos];
			break;
		}
	}
	
	// Determine start and end roll chars
	if (isAlt) {
		startRollCharPos = (startCharPos + (int)perimeterChars[startCharPos] + (int)perimeterChars[startCharPos - (GRID_SIZE*4)]) % (GRID_SIZE * 8);
		endRollCharPos = (endCharPos + (int)perimeterChars[endCharPos] + (int)perimeterChars[endCharPos - (GRID_SIZE*4)]) % (GRID_SIZE * 8);
	} else {
		startRollCharPos = (startCharPos + (int)perimeterChars[startCharPos] + (int)perimeterChars[startCharPos + (GRID_SIZE*4)]) % (GRID_SIZE * 8);
		endRollCharPos = (endCharPos + (int)perimeterChars[endCharPos] + (int)perimeterChars[endCharPos + (GRID_SIZE*4)]) % (GRID_SIZE * 8);
	}
	
	// Characters can't roll to their own position, to the other char position, or to their previous position
	while (startRollCharPos == startCharPos || startRollCharPos == endCharPos || startRollCharPos == lastStartCharPos) {
		startRollCharPos = (startRollCharPos + GRID_SIZE) % (GRID_SIZE * 4);
	}
	while (endRollCharPos == endCharPos || endRollCharPos == startCharPos || endRollCharPos == lastEndCharPos) {
		endRollCharPos = (endRollCharPos + GRID_SIZE) % (GRID_SIZE * 4);
	}
	
	// Characters can't roll to the same position
	if (startRollCharPos == endRollCharPos) {
		// do nothing for now...
	} else {

		// Roll start char
		tempChar = perimeterChars[startCharPos];
		perimeterChars[startCharPos] = perimeterChars[startRollCharPos];
		perimeterChars[startRollCharPos] = tempChar;
		
		// Remember start position for next char
		lastStartCharPos = startCharPos;
		
		// Roll end char
		tempChar = perimeterChars[endCharPos];
		perimeterChars[endCharPos] = perimeterChars[endRollCharPos];
		perimeterChars[endRollCharPos] = tempChar;
		
		// Remember end position for next char
		lastEndCharPos = endCharPos;
	
	}

	// Rotate visited mirrors and reset to zero
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {
			if (grid[r][c].visited) {
				if (grid[r][c].mirrorType == MIRROR_BACKWARD)
					grid[r][c].mirrorType = MIRROR_FORWARD;
				else if (grid[r][c].mirrorType == MIRROR_FORWARD)
					grid[r][c].mirrorType = MIRROR_STRAIGHT;
				else if (grid[r][c].mirrorType == MIRROR_STRAIGHT)
					grid[r][c].mirrorType = MIRROR_BACKWARD;

				grid[r][c].visited = 0;
			}
		}
	}	
	
	// Return crypted char
	return ech;
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
	
	for (r = -2; r <= GRID_SIZE+1; ++r) {

		for (c = -2; c <= GRID_SIZE+1; ++c) {
			
			// Highlight cell if r/c match
			if (r == pos_r && c == pos_c) {
				printf("\x1B[30m"); // foreground black
				printf("\x1B[47m"); // background white
			}
			
			// Print apropriate mirror field character
			if (r == -2 && c == -2)                     // Upper left corner
				printf("%2c", ' ');
			else if (r == -2 && c == -1)                // Upper left corner
				printf("%2c", ' ');
			else if (r == -1 && c == -2)                // Upper left corner
				printf("%2c", ' ');
			else if (r == -1 && c == -1)                // Upper left corner
				printf("%2c", ' ');
			else if (r == -1 && c == GRID_SIZE)         // Upper right corner
				printf("%2c", ' ');
			else if (r == -2 && c == GRID_SIZE)         // Upper right corner
				printf("%2c", ' ');
			else if (r == -1 && c == GRID_SIZE+1)       // Upper right corner
				printf("%2c", ' ');
			else if (r == -2 && c == GRID_SIZE+1)       // Upper right corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE && c == -1)         // Lower left corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE && c == -2)         // Lower left corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE+1 && c == -1)       // Lower left corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE+1 && c == -2)       // Lower left corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE && c == GRID_SIZE)  // Lower right corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE && c == GRID_SIZE+1)  // Lower right corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE+1 && c == GRID_SIZE)  // Lower right corner
				printf("%2c", ' ');
			else if (r == GRID_SIZE+1 && c == GRID_SIZE+1)  // Lower right corner
				printf("%2c", ' ');
			else if (r == -2)                           // Alt Top chars
				printf("%2x", perimeterChars[c + (GRID_SIZE * 4)]);
			else if (r == -1)                           // Top chars
				printf("%2x", perimeterChars[c]);
			else if (c == GRID_SIZE+1)                    // Alt Right chars
				printf("%2x", perimeterChars[r + (GRID_SIZE * 5)]);
			else if (c == GRID_SIZE)                    // Right chars
				printf("%2x", perimeterChars[r + GRID_SIZE]);
			else if (r == GRID_SIZE+1)                    // Alt Bottom chars
				printf("%2x", perimeterChars[c + (GRID_SIZE * 7)]);
			else if (r == GRID_SIZE)                    // Bottom chars
				printf("%2x", perimeterChars[c + (GRID_SIZE * 3)]);
			else if (c == -2)                           // Alt Left chars
				printf("%2x", perimeterChars[r + (GRID_SIZE * 6)]);
			else if (c == -1)                           // Left chars
				printf("%2x", perimeterChars[r + (GRID_SIZE * 2)]);
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


