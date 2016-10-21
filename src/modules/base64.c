// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "modules/base64.h"

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

base64 base64_decode(base64 data) {
	int i, f = 0;
	uint32_t octet_1, octet_2, octet_3, octet_4;
	uint32_t combined = 0;
	
	// Change encoded chars to decimal index in encoding_table
	for (i = 0; i < 64; ++i)
		if (data.encoded[0] == encoding_table[i]) {
			data.encoded[0] = i;
			++f;
			break;
		}
	for (i = 0; i < 64; ++i)
		if (data.encoded[1] == encoding_table[i]) {
			data.encoded[1] = i;
			++f;
			break;
		}
	for (i = 0; i < 64; ++i)
		if (data.encoded[2] == encoding_table[i]) {
			data.encoded[2] = i;
			++f;
			break;
		} else if (data.encoded[2] == '=') {
			data.encoded[2] = 0;
			++f;
			break;
		}
	for (i = 0; i < 64; ++i)
		if (data.encoded[3] == encoding_table[i]) {
			data.encoded[3] = i;
			++f;
			break;
		} else if (data.encoded[3] == '=') {
			data.encoded[3] = 0;
			++f;
			break;
		}
	
	// Verify all input chars were found, return with error if not
	if (f < 4) {
		data.error = 1;
		return data;
	}
	
	// Assigning octets
	octet_1 = (unsigned char)data.encoded[0];
	octet_2 = (unsigned char)data.encoded[1];
	octet_3 = (unsigned char)data.encoded[2];
	octet_4 = (unsigned char)data.encoded[3];
	
	// Combine octets into a single 32 bit int
	combined = (octet_1 << 18) + (octet_2 << 12) + (octet_3 << 6) + octet_4;
	
	data.decoded[0] = (combined >> 16) & 0xFF;
	data.decoded[1] = (combined >> 8) & 0xFF;
	data.decoded[2] = (combined >> 0) & 0xFF;
	
	return data;
}
