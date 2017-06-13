// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GPL License. See LICENSE for more details.

#ifndef MIRRORFIELD_H
#define MIRRORFIELD_H 1

/*
 * Function Prototypes
 */
void mirrorfield_init(void);
int  mirrorfield_set(unsigned char);
int  mirrorfield_validate(void);
void mirrorfield_link(void);
unsigned char mirrorfield_crypt_char(unsigned char, int);

#endif
