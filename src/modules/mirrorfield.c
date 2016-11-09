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
	unsigned char charUp;
	unsigned char charUpAlt;
	unsigned char charDown;
	unsigned char charDownAlt;
	unsigned char charLeft;
	unsigned char charLeftAlt;
	unsigned char charRight;
	unsigned char charRightAlt;
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
			grid[r][c].visited = 0;
			grid[r][c].charUp = 0;
			grid[r][c].charUpAlt = 0;
			grid[r][c].charDown = 0;
			grid[r][c].charDownAlt = 0;
			grid[r][c].charLeft = 0;
			grid[r][c].charLeftAlt = 0;
			grid[r][c].charRight = 0;
			grid[r][c].charRightAlt = 0;
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
	if (i - (GRID_SIZE * GRID_SIZE) < GRID_SIZE * 8) {
		t = i - (GRID_SIZE * GRID_SIZE);

		if (t < GRID_SIZE)
			grid[0][t].charUp = ch;
		else if (t < GRID_SIZE * 2)
			grid[t-GRID_SIZE][GRID_SIZE-1].charRight = ch;
		else if (t < GRID_SIZE * 3)
			grid[t-(GRID_SIZE*2)][0].charLeft = ch;
		else if (t < GRID_SIZE * 4)
			grid[GRID_SIZE-1][t-(GRID_SIZE*3)].charDown = ch;
		else if (t < GRID_SIZE * 5)
			grid[0][t-(GRID_SIZE*4)].charUpAlt = ch;
		else if (t < GRID_SIZE * 6)
			grid[t-(GRID_SIZE*5)][GRID_SIZE-1].charRightAlt = ch;
		else if (t < GRID_SIZE * 7)
			grid[t-(GRID_SIZE*6)][0].charLeftAlt = ch;
		else if (t < GRID_SIZE * 8)
			grid[GRID_SIZE-1][t-(GRID_SIZE*7)].charDownAlt = ch;

		return 1;
	}
	
	return 0;
}

int mirrorfield_validate(void) {
	int i, i2, r, c;
	unsigned char perimeters[GRID_SIZE * 8];
	
	// init perimeters array
	memset(perimeters, 0, GRID_SIZE * 8);
	
	// init i
	i = 0;

	// Traverse grid
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {

			// Store top chars
			if (r == 0) {
				perimeters[i++] = grid[r][c].charUp;
				perimeters[i++] = grid[r][c].charUpAlt;
			}

			// Store right chars
			if (c == GRID_SIZE - 1) {
				perimeters[i++] = grid[r][c].charRight;
				perimeters[i++] = grid[r][c].charRightAlt;
			}

			// Store left chars
			if (c == 0) {
				perimeters[i++] = grid[r][c].charLeft;
				perimeters[i++] = grid[r][c].charLeftAlt;
			}

			// Store bottom chars
			if (r == GRID_SIZE - 1) {
				perimeters[i++] = grid[r][c].charDown;
				perimeters[i++] = grid[r][c].charDownAlt;
			}

			// Check mirror type
			if (!grid[r][c].mirrorType || !strchr(SUPPORTED_MIRROR_TYPES, grid[r][c].mirrorType))
				return 0;
			
		}
	}
	
	// Check for duplicate perimeter chars
	for (i = 0; i < GRID_SIZE * 8; ++i)
		for (i2 = i+1; i2 < GRID_SIZE * 8; ++i2)
			if (perimeters[i] == perimeters[i2])
				return 0;
	
	return 1;
}

unsigned char mirrorfield_crypt_char(unsigned char ch, int debug) {
	int r, c;
	unsigned char ech;
	int direction = 0;
	int isAlt = 0;
	unsigned char *startChar;
	unsigned char *startCharAlt;
	int            startCharPos;
	unsigned char *endChar;
	unsigned char *endCharAlt;
	int            endCharPos;
	unsigned char *rollChar;
	unsigned char *rollCharAlt;
	int            rollCharPos;
	unsigned char tempChar;
	unsigned char tempCharAlt;
	
	// For the debug flag
	struct timespec ts;
	ts.tv_sec = debug / 1000;
	ts.tv_nsec = (debug % 1000) * 1000000;
	
	// Determining starting row/col and starting direction
	if (!direction) {
		for (r = 0, c = 0; c < GRID_SIZE; ++c) {
			if (grid[r][c].charUp == ch || grid[r][c].charUpAlt == ch) {
				direction = DIR_DOWN;
				startCharPos = c;
				startChar = &(grid[r][c].charUp);
				startCharAlt = &(grid[r][c].charUpAlt);
				if (grid[r][c].charUpAlt == ch)
					isAlt = 1;
				break;
			}
		}
	}
	if (!direction) {
		for (r = GRID_SIZE-1, c = 0; c < GRID_SIZE; ++c) {
			if (grid[r][c].charDown == ch || grid[r][c].charDownAlt == ch) {
				direction = DIR_UP;
				startCharPos = c + (GRID_SIZE*3);
				startChar = &(grid[r][c].charDown);
				startCharAlt = &(grid[r][c].charDownAlt);
				if (grid[r][c].charDownAlt == ch)
					isAlt = 1;
				break;
			}
		}
	}
	if (!direction) {
		for (r = 0, c = 0; r < GRID_SIZE && !direction; ++r) {
			if (grid[r][c].charLeft == ch || grid[r][c].charLeftAlt == ch) {
				direction = DIR_RIGHT;
				startCharPos = r + (GRID_SIZE*2);
				startChar = &(grid[r][c].charLeft);
				startCharAlt = &(grid[r][c].charLeftAlt);
				if (grid[r][c].charLeftAlt == ch)
					isAlt = 1;
				break;
			}
		}
	}
	if (!direction) {
		for (r = 0, c = GRID_SIZE-1; r < GRID_SIZE; ++r) {
			if (grid[r][c].charRight == ch || grid[r][c].charRightAlt == ch) {
				direction = DIR_LEFT;
				startCharPos = r + GRID_SIZE;
				startChar = &(grid[r][c].charRight);
				startCharAlt = &(grid[r][c].charRightAlt);
				if (grid[r][c].charRightAlt == ch)
					isAlt = 1;
				break;
			}
		}
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
			endChar = &(grid[0][c].charUp);
			endCharAlt = &(grid[0][c].charUpAlt);
			endCharPos = c;
			ech = isAlt ? *endCharAlt : *endChar;
			break;
		} else if (r >= GRID_SIZE) {
			endChar = &(grid[GRID_SIZE-1][c].charDown);
			endCharAlt = &(grid[GRID_SIZE-1][c].charDownAlt);
			endCharPos = c + (GRID_SIZE*3);
			ech = isAlt ? *endCharAlt : *endChar;
			break;
		} else if (c < 0) {
			endChar = &(grid[r][0].charLeft);
			endCharAlt = &(grid[r][0].charLeftAlt);
			endCharPos = r + (GRID_SIZE*2);
			ech = isAlt ? *endCharAlt : *endChar;
			break;
		} else if (c >= GRID_SIZE) {
			endChar = &(grid[r][GRID_SIZE-1].charRight);
			endCharAlt = &(grid[r][GRID_SIZE-1].charRightAlt);
			endCharPos = r + GRID_SIZE;
			ech = isAlt ? *endCharAlt : *endChar;
			break;
		}
	}
	
	// Swap and criss-cross start and end chars
	tempChar = *startChar;
	tempCharAlt = *startCharAlt;
	*startChar = *endCharAlt;
	*startCharAlt = *endChar;
	*endChar = tempCharAlt;
	*endCharAlt = tempChar;

	// Only roll characters if they are not equal
	if (((int)*startChar + (int)*startCharAlt) != ((int)*endChar + (int)*endCharAlt)) {
	
		// Determine roll character
		if (((int)*startChar + (int)*startCharAlt) > ((int)*endChar + (int)*endCharAlt)) {
			rollCharPos = (endCharPos + (int)*startChar + (int)*startCharAlt) % (GRID_SIZE * 4);
		} else {
			rollCharPos = (startCharPos + (int)*endChar + (int)*endCharAlt) % (GRID_SIZE * 4);
		}
		if (rollCharPos < GRID_SIZE) {
			rollChar = &(grid[0][rollCharPos].charUp);
			rollCharAlt = &(grid[0][rollCharPos].charUpAlt);
		} else if (rollCharPos < GRID_SIZE * 2) {
			rollChar = &(grid[rollCharPos % GRID_SIZE][GRID_SIZE-1].charRight);
			rollCharAlt = &(grid[rollCharPos % GRID_SIZE][GRID_SIZE-1].charRightAlt);
		} else if (rollCharPos < GRID_SIZE * 3) {
			rollChar = &(grid[rollCharPos % GRID_SIZE][0].charLeft);
			rollCharAlt = &(grid[rollCharPos % GRID_SIZE][0].charLeftAlt);
		} else if (rollCharPos < GRID_SIZE * 4) {
			rollChar = &(grid[GRID_SIZE-1][rollCharPos % GRID_SIZE].charDown);
			rollCharAlt = &(grid[GRID_SIZE-1][rollCharPos % GRID_SIZE].charDownAlt);
		}
		
		
		// Roll characters
		if (((int)*startChar + (int)*startCharAlt) > ((int)*endChar + (int)*endCharAlt)) {
			tempChar = *rollChar;
			tempCharAlt = *rollCharAlt;
			*rollChar = *startChar;
			*rollCharAlt = *startCharAlt;
			*startChar = tempChar;
			*startCharAlt = tempCharAlt;
		} else {
			tempChar = *rollChar;
			tempCharAlt = *rollCharAlt;
			*rollChar = *endChar;
			*rollCharAlt = *endCharAlt;
			*endChar = tempChar;
			*endCharAlt = tempCharAlt;
		}
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

void mirrorfield_roll(void) {
	int i, p;
	unsigned char c;
	unsigned char l;
	int perimeterChars[GRID_SIZE * 8];
	
	// Initialize perimeter array
	for (i = 0; i < GRID_SIZE * 8; ++i)
		perimeterChars[i] = -1;
	
	// Need last grid char to start with
	l = grid[GRID_SIZE - 1][GRID_SIZE - 1].charDown;
	
	// Re-mix the perimeter characters
	for (i = 0; i < GRID_SIZE * 8; ++i) {
		
		// Get grid char
		if (i < GRID_SIZE) {
			c = grid[0][i].charUp;
		} else if (i < GRID_SIZE * 2) {
			c = grid[i - GRID_SIZE][GRID_SIZE - 1].charRight;
		} else if (i < GRID_SIZE * 3) {
			c = grid[i - (GRID_SIZE * 2)][0].charLeft;
		} else if (i < GRID_SIZE * 4) {
			c = grid[GRID_SIZE - 1][i - (GRID_SIZE * 3)].charDown;
		} else if (i < GRID_SIZE * 5) {
			c = grid[0][i - (GRID_SIZE * 4)].charUpAlt;
		} else if (i < GRID_SIZE * 6) {
			c = grid[i - (GRID_SIZE * 5)][GRID_SIZE - 1].charRightAlt;
		} else if (i < GRID_SIZE * 7) {
			c = grid[i - (GRID_SIZE * 6)][0].charLeftAlt;
		} else if (i < GRID_SIZE * 8) {
			c = grid[GRID_SIZE - 1][i - (GRID_SIZE * 7)].charDownAlt;
		}
		
		// Determine next grid position
		p = (int)c + (int)l;
		if (p >= (GRID_SIZE * 8))
			p -= (GRID_SIZE * 8);
		
		// If position is taken, increment until we find an empty spot
		while (perimeterChars[p] >= 0) {
			++p;
			if (p > (GRID_SIZE * 8) - 1)
				p = 0;
		}
		
		// Set new character position
		perimeterChars[p] = (int)c;
		
		// Store c to be used in next interation
		l = c;
	}
	
	// Write mixed perimeter chars to grid
	for (i = 0; i < GRID_SIZE * 8; ++i) {
		if (i < GRID_SIZE) {
			grid[0][i].charUp = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 2) {
			grid[i - GRID_SIZE][GRID_SIZE - 1].charRight = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 3) {
			grid[i - (GRID_SIZE * 2)][0].charLeft = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 4) {
			grid[GRID_SIZE - 1][i - (GRID_SIZE * 3)].charDown = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 5) {
			grid[0][i - (GRID_SIZE * 4)].charUpAlt = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 6) {
			grid[i - (GRID_SIZE * 5)][GRID_SIZE - 1].charRightAlt = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 7) {
			grid[i - (GRID_SIZE * 6)][0].charLeftAlt = (unsigned char)perimeterChars[i];
		} else if (i < GRID_SIZE * 8) {
			grid[GRID_SIZE - 1][i - (GRID_SIZE * 7)].charDownAlt = (unsigned char)perimeterChars[i];
		}
	}
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
				printf("%2x", grid[r+2][c].charUpAlt);
			else if (r == -1)                           // Top chars
				printf("%2x", grid[r+1][c].charUp);
			else if (c == GRID_SIZE+1)                    // Alt Right chars
				printf("%2x", grid[r][c-2].charRightAlt);
			else if (c == GRID_SIZE)                    // Right chars
				printf("%2x", grid[r][c-1].charRight);
			else if (r == GRID_SIZE+1)                    // Alt Bottom chars
				printf("%2x", grid[r-2][c].charDownAlt);
			else if (r == GRID_SIZE)                    // Bottom chars
				printf("%2x", grid[r-1][c].charDown);
			else if (c == -2)                           // Alt Left chars
				printf("%2x", grid[r][c+2].charLeftAlt);
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


