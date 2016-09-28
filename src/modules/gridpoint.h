// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef GRIDPOINT_H
#define GRIDPOINT_H 1

void gridpoint_init(void);
unsigned int gridpoint_get_width(void);
void gridpoint_set_type(int, int, int);
void gridpoint_set_charup(int, int, char);
void gridpoint_set_chardown(int, int, char);
void gridpoint_set_charleft(int, int, char);
void gridpoint_set_charright(int, int, char);
int  gridpoint_get_type(int, int);
char gridpoint_get_charup(int, int);
char gridpoint_get_chardown(int, int);
char gridpoint_get_charleft(int, int);
char gridpoint_get_charright(int, int);

#endif
