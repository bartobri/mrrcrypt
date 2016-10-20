// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef BASE64_H
#define BASE64_H 1

#define B64_NOFORCE 0
#define B64_FORCE   1

char *base64_encode_char(unsigned char, int);
char *base64_encode(unsigned char, unsigned char, unsigned char, int);
unsigned char *base64_decode_char(char);
unsigned char *base64_decode(char, char, char, char);

#endif
