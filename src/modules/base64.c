// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "modules/base64.h"

#define DECODE_INPUT_COUNT    4
#define DECODE_OUTPUT_COUNT   3

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

base64 base64_encode(base64 data) {
	uint32_t octet_1, octet_2, octet_3;
	uint32_t combined = 0;
	
	// Assigning octets
	octet_1 = data.index >= 1 ? data.decoded[0] : 0;
	octet_2 = data.index >= 2 ? data.decoded[1] : 0;
	octet_3 = data.index >= 3 ? data.decoded[2] : 0;

	// Combine octets into a single 32 bit int
	combined = (octet_1 << 16) + (octet_2 << 8) + octet_3;
	
	// Generate encoded chars
	data.encoded[0] = encoding_table[(combined >> 18) & 0x3F];
	data.encoded[1] = encoding_table[(combined >> 12) & 0x3F];
	data.encoded[2] = encoding_table[(combined >> 6) & 0x3F];
	data.encoded[3] = encoding_table[(combined >> 0) & 0x3F];
	
	// Setting trailing chars '=' in accordance with base64 encoding standard
	if (data.index == 1) {
		data.encoded[2] = '=';
		data.encoded[3] = '=';
	} else if (data.index == 2) {
		data.encoded[3] = '=';
	}
	
	return data;
}

unsigned char *base64_decode_char(char c) {
	static unsigned char *out = NULL;
	static char in[DECODE_INPUT_COUNT];
	static int i = 0;
	
	// Allocate memory if not done yet
	if (out == NULL)
		out = malloc(DECODE_OUTPUT_COUNT);
	
	// Assign input char to next array
	in[i++] = c;
	
	// Get encoded output if we have 3 chars, or if force flag is used
	if (i == DECODE_INPUT_COUNT) {
		out = base64_decode(in[0], in[1], in[2], in[3]);
		i = 0;
	}

	return out;
}

unsigned char *base64_decode(char ch1, char ch2, char ch3, char ch4) {
	int i;
	int f = 0;
	uint32_t octet_1, octet_2, octet_3, octet_4;
	uint32_t combined = 0;
	static unsigned char out[DECODE_OUTPUT_COUNT + 1];

	// Zeroing output
	memset(out, 0, DECODE_OUTPUT_COUNT + 1);
	
	// Change char to decimal index in encoding_table
	for (i = 0; i < 64; ++i)
		if (ch1 == encoding_table[i]) {
			ch1 = i;
			++f;
			break;
		}
	for (i = 0; i < 64; ++i)
		if (ch2 == encoding_table[i]) {
			ch2 = i;
			++f;
			break;
		}
	for (i = 0; i < 64; ++i)
		if (ch3 == encoding_table[i]) {
			ch3 = i;
			++f;
			break;
		} else if (ch3 == '=') {
			ch3 = 0;
			++f;
			break;
		}
	for (i = 0; i < 64; ++i)
		if (ch4 == encoding_table[i]) {
			ch4 = i;
			++f;
			break;
		} else if (ch4 == '=') {
			ch4 = 0;
			++f;
			break;
		}
	
	// Verify all input chars were found, return empty string if not
	if (f < 4)
		return out;
	
	// Assigning octets
	octet_1 = (unsigned char)ch1;
	octet_2 = (unsigned char)ch2;
	octet_3 = (unsigned char)ch3;
	octet_4 = (unsigned char)ch4;
	
	// Combine octets into a single 32 bit int
	combined = (octet_1 << 18) + (octet_2 << 12) + (octet_3 << 6) + octet_4;
	
	out[0] = (combined >> 16) & 0xFF;
	out[1] = (combined >> 8) & 0xFF;
	out[2] = (combined >> 0) & 0xFF;
	
	return out;
}
