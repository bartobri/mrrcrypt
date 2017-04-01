// Copyright (c) 2017 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GPL License. See LICENSE for more details.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "modules/base64.h"

/*
 * MODULE DESCRIPTION
 * 
 * The base64 module was designed to work with unencoded and encoded characters
 * in a 3-4 ratio. Which is to say, every 3 unencoded characters will
 * produce 4 encoded characters. This ratio is inherent within the base64
 * specification. The base64 structure defined in base64.h was designed
 * to facilitate the 3-4 ratio.
 * 
 * Long strings that need to be encoded should be passed to the base64_encode()
 * function no more than 3 characters at a time, via the decoded[] character array
 * defined in the base64 structure. Set the structure index to the number of
 * characters you want encoded. This will usually be 3 unless there is a
 * trailing 1 or 2 characters at the end of your string.
 * 
 * The returned structure will contain 4 encoded characters stored in the
 * encoded[] character array.
 * 
 * Repeat this process until the entire string is encoded.
 * 
 * Decoding a string works the same way but in reverse. Feed your encoded
 * string into the base64_decode() function 4 characters at a time, via
 * the encoded[] character array defined in the base64 structure. There is
 * no need to set the index when decoding because a properly encoded string
 * will always be evenly divisible by 4. The decode function always assumes
 * it will receive 4 encoded characters. If it does not, it will set the
 * error flag and return.
 * 
 * The returned structure will contain 3 decoded characters stored in the
 * decoded[] character array.
 */

/*
 * Base64 Character Encoding Table
 */
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
/*
 * The base64_encode() function encodes the characters stored in the decoded[]
 * character array member of the base64 structure. Only the number of
 * characters specified by index will be encoded. Encoded characters are
 * stored in the encoded[] character array member of the returned base64
 * structure.
 */
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

/*
 * The base64_decode() function decodes the characters stored in the encoded[]
 * character array member of the base64 structure. Decoded characters are
 * stored in the decoded[] character array member of the returned base64
 * structure.
 */
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
