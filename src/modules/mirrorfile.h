// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef MIRRORFILE_H
#define MIRRORFILE_H 1

void mirrorfile_init(void);
int mirrorfile_open(char *);
int mirrorfile_create(char *, int);
int mirrorfile_next_char(void);
void mirrorfile_close(void);

#endif
