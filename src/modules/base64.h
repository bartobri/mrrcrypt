// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef BASE64_H
#define BASE64_H 1

#define BASE64_ENCODED_COUNT 4
#define BASE64_DECODED_COUNT 3

typedef struct {
	char encoded[BASE64_ENCODED_COUNT];
	unsigned char decoded[BASE64_DECODED_COUNT];
	int index;
	int error;
} base64;

base64 base64_encode(base64);
base64 base64_decode(base64);

#endif
