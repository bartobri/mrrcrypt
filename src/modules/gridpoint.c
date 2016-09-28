// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#define MIRROR_NONE      0
#define MIRROR_FORWARD   1
#define MIRROR_BACKWARD  2
#define GRID_WIDTH       24

// Gridpoint Structure
struct gridPoint {
	int mirrorType;
	char charUp;
	char charDown;
	char charLeft;
	char charRight;
};

// Static Variables
static struct gridPoint grid[GRID_WIDTH][GRID_WIDTH];

void gridpoint_init(void) {
	int r, c;

	// Populate Grid
	for (r = 0; r < GRID_WIDTH; ++r) {
		for (c = 0; c < GRID_WIDTH; ++c) {

			// Init struct with default values
			grid[r][c].mirrorType = MIRROR_NONE;
			grid[r][c].charUp = 0;
			grid[r][c].charDown = 0;
			grid[r][c].charLeft = 0;
			grid[r][c].charRight = 0;
		}
	}
}

void gridpoint_set_type(int r, int c, int value) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		grid[r][c].mirrorType = value;
}

void gridpoint_set_charup(int r, int c, char value) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		grid[r][c].charUp = value;
}

void gridpoint_set_chardown(int r, int c, char value) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		grid[r][c].charDown = value;
}

void gridpoint_set_charleft(int r, int c, char value) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		grid[r][c].charLeft = value;
}

void gridpoint_set_charright(int r, int c, char value) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		grid[r][c].charRight = value;
}

int gridpoint_get_type(int r, int c) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		return grid[r][c].mirrorType;
	
	return 0;
}

char gridpoint_get_charup(int r, int c) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		return grid[r][c].charUp;
	
	return 0;
}

char gridpoint_get_chardown(int r, int c) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		return grid[r][c].charDown;
	
	return 0;
}

char gridpoint_get_charleft(int r, int c) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		return grid[r][c].charLeft;
	
	return 0;
}

char gridpoint_get_charright(int r, int c) {
	if (r < GRID_WIDTH && c < GRID_WIDTH)
		return grid[r][c].charRight;
	
	return 0;
}


