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


