// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include "main.h"

// Gridpoint Structure
struct gridPoint {
	int mirrorType;
	char charUp;
	char charDown;
	char charLeft;
	char charRight;
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
		}
	}
}

void mirrorfield_set_type(int r, int c, int value) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		grid[r][c].mirrorType = value;
}

/*
 * Adds character to the next empty perimter slot in the order of
 * top, right, left, down.
 */
void mirrorfield_set_char(char value) {
	int i;
	
	for (i = 0; i < GRID_SIZE * 4; ++i) {
		
		// top
		if (i < GRID_SIZE) {
			if (grid[0][i].charUp == 0) {
				grid[0][i].charUp = value;
				break;
			}
			continue;
		}
		
		// right
		if (i < GRID_SIZE * 2) {
			if (grid[i-GRID_SIZE][GRID_SIZE-1].charRight == 0) {
				grid[i-GRID_SIZE][GRID_SIZE-1].charRight = value;
				break;
			}
			continue;
		}
		
		// left
		if (i < GRID_SIZE * 3) {
			if (grid[i-(GRID_SIZE*2)][0].charLeft == 0) {
				grid[i-(GRID_SIZE*2)][0].charLeft = value;
				break;
			}
			continue;
		}
		
		// top
		if (i < GRID_SIZE * 4) {
			if (grid[GRID_SIZE-1][i-(GRID_SIZE*3)].charDown == 0) {
				grid[GRID_SIZE-1][i-(GRID_SIZE*3)].charDown = value;
				break;
			}
			continue;
		}
	}
}

void mirrorfield_set_charup(int r, int c, char value) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		grid[r][c].charUp = value;
}

void mirrorfield_set_chardown(int r, int c, char value) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		grid[r][c].charDown = value;
}

void mirrorfield_set_charleft(int r, int c, char value) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		grid[r][c].charLeft = value;
}

void mirrorfield_set_charright(int r, int c, char value) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		grid[r][c].charRight = value;
}

int mirrorfield_get_type(int r, int c) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		return grid[r][c].mirrorType;
	
	return 0;
}

char mirrorfield_get_charup(int r, int c) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		return grid[r][c].charUp;
	
	return 0;
}

char mirrorfield_get_chardown(int r, int c) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		return grid[r][c].charDown;
	
	return 0;
}

char mirrorfield_get_charleft(int r, int c) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		return grid[r][c].charLeft;
	
	return 0;
}

char mirrorfield_get_charright(int r, int c) {
	if (r < GRID_SIZE && c < GRID_SIZE)
		return grid[r][c].charRight;
	
	return 0;
}

int mirrorfield_has_char(char ch) {
	int i;

	for (i = 0; i < GRID_SIZE * 4; ++i) {
		
		// top
		if (i < GRID_SIZE) {
			if (grid[0][i].charUp == ch)
				return 1;

			continue;
		}
		
		// right
		if (i < GRID_SIZE * 2) {
			if (grid[i-GRID_SIZE][GRID_SIZE-1].charRight == ch)
				return 1;

			continue;
		}
		
		// left
		if (i < GRID_SIZE * 3) {
			if (grid[i-(GRID_SIZE*2)][0].charLeft == ch)
				return 1;

			continue;
		}
		
		// top
		if (i < GRID_SIZE * 4) {
			if (grid[GRID_SIZE-1][i-(GRID_SIZE*3)].charDown == ch)
				return 1;

			continue;
		}
	}
	
	return 0;
}

void mirrorfield_rotate() {
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


