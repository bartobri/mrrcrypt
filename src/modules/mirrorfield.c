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
	char charUp;
	char charDown;
	char charLeft;
	char charRight;
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

int mirrorfield_set(int i, char ch) {
	
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
	if (i - (GRID_SIZE * GRID_SIZE) < (int)strlen(SUPPORTED_CHARS)) {
		i -= (GRID_SIZE * GRID_SIZE);
		if (ch != '\0' && strchr(SUPPORTED_CHARS, ch)) {
			if (i < GRID_SIZE)
				grid[0][i].charUp = ch;
			else if (i < GRID_SIZE * 2)
				grid[i-GRID_SIZE][GRID_SIZE-1].charRight = ch;
			else if (i < GRID_SIZE * 3)
				grid[i-(GRID_SIZE*2)][0].charLeft = ch;
			else if (i < GRID_SIZE * 4)
				grid[GRID_SIZE-1][i-(GRID_SIZE*3)].charDown = ch;
				
			return 1;
		} else {
			return 0;
		}
	}
	
	return 0;
}

int mirrorfield_validate(void) {
	int i, r, c;
	char perimeters[GRID_SIZE * 4];
	
	// init perimeters array
	memset(perimeters, 0, GRID_SIZE * 4);
	
	// init i
	i = 0;

	// Traverse grid
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {

			// Check top chars
			if (r == 0) {
				if (!grid[r][c].charUp || !strchr(SUPPORTED_CHARS, grid[r][c].charUp))
					return 0;

				if (strchr(perimeters, grid[r][c].charUp))
					return 0;
				else
					perimeters[i++] = grid[r][c].charUp;
			}
			
			// Check right chars
			if (c == GRID_SIZE - 1) {
				if (!grid[r][c].charRight || !strchr(SUPPORTED_CHARS, grid[r][c].charRight))
					return 0;
				
				if (strchr(perimeters, grid[r][c].charRight))
					return 0;
				else
					perimeters[i++] = grid[r][c].charRight;
			}
			
			// Check left chars
			if (c == 0) {
				if (!grid[r][c].charLeft || !strchr(SUPPORTED_CHARS, grid[r][c].charLeft))
					return 0;
				
				if (strchr(perimeters, grid[r][c].charLeft))
					return 0;
				else
					perimeters[i++] = grid[r][c].charLeft;
			}
			
			// Check bottom chars
			if (r == GRID_SIZE - 1) {
				if (!grid[r][c].charDown || !strchr(SUPPORTED_CHARS, grid[r][c].charDown))
					return 0;
				
				if (strchr(perimeters, grid[r][c].charDown))
					return 0;
				else
					perimeters[i++] = grid[r][c].charDown;
			}
			
			// Check mirror type
			if (!grid[r][c].mirrorType || !strchr(SUPPORTED_MIRROR_TYPES, grid[r][c].mirrorType))
				return 0;
			
		}
	}
	
	return 1;
}

char mirrorfield_crypt_char(char ch, int debug) {
	int r, c;
	int ech = 0;
	int direction = 0;
	struct timespec ts;
	
	if (debug) {
		ts.tv_sec = debug / 1000;
		ts.tv_nsec = (debug % 1000) * 1000000;
	}
	
	// Clear visited mirror points
	for (r = 0; r < GRID_SIZE; ++r)
		for (c = 0; c < GRID_SIZE; ++c)
			grid[r][c].visited = 0;
	
	// Determining starting row/col and starting direction
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {
			if (grid[r][c].charUp == ch)
				direction = DIR_DOWN;
			else if (grid[r][c].charLeft == ch)
				direction = DIR_RIGHT;
			else if (grid[r][c].charRight == ch)
				direction = DIR_LEFT;
			else if (grid[r][c].charDown == ch)
				direction = DIR_UP;
			
			if (direction)
				break;
		}
		if (direction)
			break;
	}

	// Traverse through the grid
	while (ech == 0) {
		
		// Draw mirror field if debug flag is set
		if (debug) {
			mirrorfield_draw(r, c);
			nanosleep(&ts, NULL);
		}
		
		// If we hit a mirror that we've already been to, un-rotate it.
		// This is necessary to preserve the ability to reverse the encryption.
		// We can not rotate a mirror more than once per character.
		if (grid[r][c].mirrorType != MIRROR_NONE) {
			if (grid[r][c].visited) {
				if (grid[r][c].mirrorType == MIRROR_FORWARD) {
					grid[r][c].mirrorType = MIRROR_BACKWARD;
				} else if (grid[r][c].mirrorType == MIRROR_BACKWARD) {
					grid[r][c].mirrorType = MIRROR_STRAIGHT;
				} else if (grid[r][c].mirrorType == MIRROR_STRAIGHT) {
					grid[r][c].mirrorType = MIRROR_FORWARD;
				}
			} else {
				grid[r][c].visited = 1;
			}
		}

		// Forward mirror / Change direction and rotate
		if (grid[r][c].mirrorType == MIRROR_FORWARD) {
			if (direction == DIR_DOWN)
				direction = DIR_LEFT;
			else if (direction == DIR_LEFT)
				direction = DIR_DOWN;
			else if (direction == DIR_RIGHT)
				direction = DIR_UP;
			else if (direction == DIR_UP)
				direction = DIR_RIGHT;
			
			// Rotate mirror
			grid[r][c].mirrorType = MIRROR_STRAIGHT;
		}
		
		// Straight mirror - Keep same direction, just rotate
		else if (grid[r][c].mirrorType == MIRROR_STRAIGHT) {

			// Rotate mirror
			grid[r][c].mirrorType = MIRROR_BACKWARD;
		}

		// Backward mirror \ Change direction and rotate
		else if (grid[r][c].mirrorType == MIRROR_BACKWARD) {
			if (direction == DIR_DOWN)
				direction = DIR_RIGHT;
			else if (direction == DIR_LEFT)
				direction = DIR_UP;
			else if (direction == DIR_RIGHT)
				direction = DIR_DOWN;
			else if (direction == DIR_UP)
				direction = DIR_LEFT;
			
			// Rotate mirror
			grid[r][c].mirrorType = MIRROR_FORWARD;
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
		else if (r >= GRID_SIZE)
			ech = grid[GRID_SIZE - 1][c].charDown;
		else if (c < 0)
			ech = grid[r][0].charLeft;
		else if (c >= GRID_SIZE)
			ech = grid[r][GRID_SIZE - 1].charRight;
	}
	
	return ech;
}

void mirrorfield_rotate(void) {
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
	
	// Clear screen
	printf("\033[H\033[J");
	
	// Position cursor
	printf("\033[%d;%dH", 1, 1);
	
	for (r = -1; r <= GRID_SIZE; ++r) {

		//wmove(wGrid, r + 3, (termSizeCols - (GRID_SIZE * 2)) / 2);

		for (c = -1; c <= GRID_SIZE; ++c) {
			
			// Highlight cell if r/c match
			if (r == pos_r && c == pos_c) {
				printf("\x1B[30m"); // foreground black
				printf("\x1B[47m"); // background white
			}
			
			// Print apropriate mirror field character
			if (r == -1 && c == -1)                     // Upper left corner
				printf(" ");
			else if (r == -1 && c == GRID_SIZE)         // Upper right corner
				printf(" ");
			else if (r == GRID_SIZE && c == -1)         // Lower left corner
				printf(" ");
			else if (r == GRID_SIZE && c == GRID_SIZE)  // Lower right corner
				printf(" ");
			else if (r == -1) {                           // Top chars
				if (isspace(grid[r+1][c].charUp))
					printf(" ");
				else
					printf("%c", grid[r+1][c].charUp);
			} else if (c == GRID_SIZE) {                   // Right chars
				if (isspace(grid[r][c-1].charRight))
					printf(" ");
				else
					printf("%c", grid[r][c-1].charRight);
			} else if (r == GRID_SIZE) {                  // Bottom chars
				if (isspace(grid[r-1][c].charDown))
					printf(" ");
				else
					printf("%c", grid[r-1][c].charDown);
			} else if (c == -1) {                         // Left chars
				if (isspace(grid[r][c+1].charLeft))
					printf(" ");
				else
					printf("%c", grid[r][c+1].charLeft);
			} else if (grid[r][c].mirrorType == MIRROR_FORWARD)
				printf("/");
			else if (grid[r][c].mirrorType == MIRROR_BACKWARD)
				printf("\\");
			else if (grid[r][c].mirrorType == MIRROR_STRAIGHT)
				printf("-");
			else
				printf(" ");
			
			// Un-Highlight cell if r/c match
			if (r == pos_r && c == pos_c)
				printf("\x1B[0m");
			
			//printf(" ");

		}
		printf("\n");
	}
	printf("\n");
}


