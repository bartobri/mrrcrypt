// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <string.h>

#define SUPPORTED_CHARS  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+{}[]|\\:;\"'<>,.?/~\n\t "

// Static Variables
static char *supportedChars;

void supportedchars_init(void) {
	supportedChars = SUPPORTED_CHARS;
}

unsigned int supportedchars_count(void) {
	return strlen(supportedChars);
}

char supportedchars_get(int i) {
	return supportedChars[i];
}
