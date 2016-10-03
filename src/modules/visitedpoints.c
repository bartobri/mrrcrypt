// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#define MAX_POINTS 100

struct point {
	int v;
	int r;
	int c;
};

static struct point visited[MAX_POINTS];

void visitedpoints_init(void) {
	int i;

	for (i = 0; i < MAX_POINTS; ++i)
		visited[i].v = 0;
}

void visitedpoints_add(int r, int c) {
	int i;

	for (i = 0; i < MAX_POINTS; ++i) {
		if (visited[i].v == 0) {
			visited[i].v = 1;
			visited[i].r = r;
			visited[i].c = c;
			break;
		}
	}
}

int visitedpoints_exists(int r, int c) {
	int i, x;
	
	x = 0;

	for (i = 0; i < MAX_POINTS; ++i) {
		if (visited[i].v == 0) {
			break;
		} else if (visited[i].r == r && visited[i].c == c) {
			x = 1;
			break;
		}
	}
	
	return x;
}

void visitedpoints_clear(void) {
	int i;

	for (i = 0; i < MAX_POINTS; ++i)
		visited[i].v = 0;
}
