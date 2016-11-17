// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef MIRRORFIELD_H
#define MIRRORFIELD_H 1

void mirrorfield_init(void);
int mirrorfield_set(unsigned char);
int mirrorfield_validate(void);
unsigned char mirrorfield_crypt_char(unsigned char, int);
void mirrorfield_roll_chars(int, int);
void mirrorfield_draw(int, int);

#endif
