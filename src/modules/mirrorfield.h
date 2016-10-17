// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef MIRRORFIELD_H
#define MIRRORFIELD_H 1

void mirrorfield_init(void);
int mirrorfield_set(int, char);
int mirrorfield_validate(void);
char mirrorfield_crypt_char(char);
void mirrorfield_rotate(void);

#endif
