// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GPL License. See LICENSE for more details.

#ifndef BASE64_H
#define BASE64_H 1

#define BASE64_ENCODED_COUNT 4
#define BASE64_DECODED_COUNT 3

/*
 * Input/Output Structure Definition
 */
typedef struct {
	char encoded[BASE64_ENCODED_COUNT];
	unsigned char decoded[BASE64_DECODED_COUNT];
	int index;
	int error;
} base64;

/*
 * Function Prototypes
 */
base64 base64_encode(base64);
base64 base64_decode(base64);

#endif
