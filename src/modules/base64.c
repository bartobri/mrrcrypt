// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "modules/base64.h"

#define INPUT_COUNT  3
#define OUTPUT_COUNT 4

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

char *base64_encode_char(char c, int force) {
	static char *out = NULL;
	static char in[INPUT_COUNT];
	static int i = 0;
	
	// Allocate memory if not done yet
	if (out == NULL)
		out = malloc(OUTPUT_COUNT + 1);
	
	// Zeroing output
	memset(out, 0, OUTPUT_COUNT + 1);
	
	// Assign input char to next array
	in[i++] = c;
	
	// Get encoded output if we have 3 chars, or if force flag is used
	if (i == INPUT_COUNT || force) {
		out = base64_encode(in[0], in[1], in[2], i);
		i = 0;
	}
	
	return out;
}

char *base64_encode(char ch1, char ch2, char ch3, int l) {
	uint32_t octet_1, octet_2, octet_3;
	uint32_t combined = 0;
	static char out[OUTPUT_COUNT + 1];
	
	// Zeroing output
	memset(out, 0, OUTPUT_COUNT + 1);
	
	// Assigning octets
	octet_1 = l >= 1 ? (unsigned char)ch1 : 0;
	octet_2 = l >= 2 ? (unsigned char)ch2 : 0;
	octet_3 = l >= 3 ? (unsigned char)ch3 : 0;

	// Combine octets into a single 32 bit int
	combined = (octet_1 << 16) + (octet_2 << 8) + octet_3;
	
	// 00000000 01010101 01010101 01010101
	
	out[0] = encoding_table[(combined >> 18) & 0x3F];
	out[1] = encoding_table[(combined >> 12) & 0x3F];
	out[2] = encoding_table[(combined >> 6) & 0x3F];
	out[3] = encoding_table[(combined >> 0) & 0x3F];
	
	// Setting trailing chars '=' in accordance with base64 encoding standard
	if (l == 1) {
		out[2] = '=';
		out[3] = '=';
	} else if (l == 2) {
		out[3] = '=';
	}
	
	return out;
}
