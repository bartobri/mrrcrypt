// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#define MAX_MIRRORS 200

struct point {
	int v;
	int r;
	int c;
};

static struct point visited[MAX_MIRRORS];

void visitedmirrors_init(void) {
	int i;

	for (i = 0; i < MAX_MIRRORS; ++i)
		visited[i].v = 0;
}

void visitedmirrors_add(int r, int c) {
	int i;

	for (i = 0; i < MAX_MIRRORS; ++i) {
		if (visited[i].v == 0) {
			visited[i].v = 1;
			visited[i].r = r;
			visited[i].c = c;
			break;
		}
	}
}

int visitedmirrors_exists(int r, int c) {
	int i, x;
	
	x = 0;

	for (i = 0; i < MAX_MIRRORS; ++i) {
		if (visited[i].v == 0) {
			break;
		} else if (visited[i].r == r && visited[i].c == c) {
			x = 1;
			break;
		}
	}
	
	return x;
}

void visitedmirrors_clear(void) {
	int i;

	for (i = 0; i < MAX_MIRRORS; ++i)
		visited[i].v = 0;
}
