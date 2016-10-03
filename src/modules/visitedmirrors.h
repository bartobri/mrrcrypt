// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef VISITEDMIRRORS_H
#define VISITEDMIRRORS_H 1

void visitedmirrors_init(void);
void visitedmirrors_add(int, int);
int visitedmirrors_exists(int, int);
void visitedmirrors_clear(void);

#endif
