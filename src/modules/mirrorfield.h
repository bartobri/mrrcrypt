// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef MIRRORFIELD_H
#define MIRRORFIELD_H 1

void mirrorfield_init(void);
void mirrorfield_set_type(int, int, int);
void mirrorfield_set_charup(int, int, char);
void mirrorfield_set_chardown(int, int, char);
void mirrorfield_set_charleft(int, int, char);
void mirrorfield_set_charright(int, int, char);
int  mirrorfield_get_type(int, int);
char mirrorfield_get_charup(int, int);
char mirrorfield_get_chardown(int, int);
char mirrorfield_get_charleft(int, int);
char mirrorfield_get_charright(int, int);
void mirrorfield_rotate();

#endif
