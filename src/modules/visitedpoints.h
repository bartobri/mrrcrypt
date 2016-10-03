// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef VISITEDPOINTS_H
#define VISITEDPOINTS_H 1

void visitedpoints_init(void);
void visitedpoints_add(int, int);
int visitedpoints_exists(int, int);
void visitedpoints_clear(void);

#endif
