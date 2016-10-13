// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef KEYFILE_H
#define KEYFILE_H 1

void  keyfile_init(void);
int   keyfile_open(char *, int);
int   keyfile_create(char *);
char *keyfile_shuffle_string(char *, int);
int   keyfile_next_char(void);
void  keyfile_close(void);

#endif
