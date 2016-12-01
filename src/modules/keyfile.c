// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#include "main.h"
#include "modules/keyfile.h"
#include "modules/base64.h"

/*
 * MODULE DESCRIPTION
 * 
 * The keyfile module manages the opening, closing, creating, and
 * reading of a key file. Creating the key file also includes creating
 * the key itself.
 */

#define MIRROR_DENSITY   20

// Static variables
static FILE *keyFile;

/*
 * The keyfile_init() function initializes any static variables.
 */
void keyfile_init(void) {
	keyFile = NULL;
}

/*
 * The keyfile_open() function attempts to open the keyfile specified in
 * the keyFileName parameter. If keyFileName does not contain an absolute
 * path, it will use $HOME/DEFAULT_KEY_PATH as the path to the directory
 * that contains the specified key file.
 * 
 * If the key file is not found and the autoCreate flag is set, it will
 * call the keyfile_create() function to attempt to create a new key file
 * at the determined path.
 * 
 * Upon any errors, zero is returned.
 */
int keyfile_open(char *keyFileName, int autoCreate) {
	char *homeDir              = getenv("HOME");
	char *keyFilePath          = DEFAULT_KEY_PATH;
	char *keyFilePathName;
	char *keyFileFullPathName;
	
	// Return if we don't have a home directory
	if (homeDir == NULL)
		return 0;
	
	// Check if we have an absolute path	
	if (*keyFileName == '/') {
	
		// Assign to full path variable
		keyFileFullPathName = keyFileName;

	} else {

		// Combine default file path and name in to one string.
		keyFilePathName = malloc(strlen(keyFilePath) + strlen(keyFileName) + 1);
		sprintf(keyFilePathName, "%s%s", keyFilePath, keyFileName);
		
		// Build key file path
		keyFileFullPathName = malloc(strlen(keyFilePathName) + strlen(homeDir) + 2);
		sprintf(keyFileFullPathName, "%s/%s", homeDir, keyFilePathName);
		
		// Build key file path
		keyFileFullPathName = malloc(strlen(keyFilePathName) + strlen(homeDir) + 2);
		sprintf(keyFileFullPathName, "%s/%s", homeDir, keyFilePathName);
	}

	// Open key file
	while ((keyFile = fopen(keyFileFullPathName, "r")) == NULL) {
		if (autoCreate) {
			if (keyfile_create(keyFileFullPathName)) {
				continue;
			} 
		} 
		break;
	}
	
	// Return zero if we can't open the key file
	if (keyFile == NULL)
		return 0;
	
	return 1;
}

/*
 * The keyfile_create() function creates a new key file at the given path.
 * A full absolute path and name must be provided as a single character
 * sting. It will attempt to create any missing directories in the path
 * if necessary.
 * 
 * Upon successfully creating the key file, it will also populate the file
 * with a new randomized key.
 * 
 * Upon any errors, zero is returned.
 */
int keyfile_create(char *keyFileFullPathName) {
	int i, r, c;
	int w = 0;
	uint32_t randseed = 0;
	struct stat sb;
	FILE *keyfile;
	FILE *urandom;
	unsigned char perimeterChars[GRID_SIZE * 4];
	base64 contents = { .index = 0 };

	// Check subdirs and create them if needed
	if (strchr(keyFileFullPathName, '/') != NULL) {
		for (i = 1; keyFileFullPathName[i] != '\0'; ++i) {
			if (keyFileFullPathName[i] == '/') {
				keyFileFullPathName[i] = '\0';
				if (stat(keyFileFullPathName, &sb) != 0)
					if (mkdir(keyFileFullPathName, 0700) == -1)
						return 0;
				keyFileFullPathName[i] = '/';
			}
		}
	}

	// Open the key file
	if ((keyfile = fopen(keyFileFullPathName, "w")) == NULL)
		return 0;
		
	// Open the urandom resource
	if ((urandom = fopen("/dev/urandom", "r")) == NULL)
		return 0;

	// Write mirror data to file
	for (r = 0; r < GRID_SIZE; ++r) {
		for (c = 0; c < GRID_SIZE; ++c) {

			// (Re)seed our PRNG
			for (i = 24; i >= 0; i -= 8)
				randseed += ((uint32_t)fgetc(urandom) << i);
			srand(randseed);

			// Randomly generate mirror char
			switch (rand() % MIRROR_DENSITY) {
				case 1:
					contents.decoded[contents.index++] = '/';
					break;
				case 2:
					contents.decoded[contents.index++] = '\\';
					break;
				default:
					contents.decoded[contents.index++] = ' ';
					break;
			}

			// If we have max input for the base64 encoder, encode it
			if (contents.index == BASE64_DECODED_COUNT) {
				contents = base64_encode(contents);
				fprintf(keyfile, "%c%c%c%c", contents.encoded[0], contents.encoded[1], contents.encoded[2], contents.encoded[3]);
				contents.index = 0;

				// Newline after every 72 chars (18 * 4)
				if (++w % 18 == 0)
					fprintf(keyfile, "\n");
			}
		}
	}
	
	// Close urandom resource
	fclose(urandom);
	
	// Generate perimeter characters
	for (i = 0; i < GRID_SIZE * 4; i++)
		perimeterChars[i] = i;
		
	// Shuffle perimeter characters
	keyfile_shuffle_string(perimeterChars, 2000);
	
	// Encode perimeter chars to base64 and write to key file
	for (i = 0; i < GRID_SIZE * 4; ++i) {
		contents.decoded[contents.index++] = perimeterChars[i];
		if (contents.index == BASE64_DECODED_COUNT) {
			contents = base64_encode(contents);
			fprintf(keyfile, "%c%c%c%c", contents.encoded[0], contents.encoded[1], contents.encoded[2], contents.encoded[3]);
			contents.index = 0;
			
			// Newline after every 72 chars (18 * 4)
			if (++w % 18 == 0)
				fprintf(keyfile, "\n");
		}
	}
	if (contents.index > 0) {
		contents = base64_encode(contents);
		fprintf(keyfile, "%c%c%c%c", contents.encoded[0], contents.encoded[1], contents.encoded[2], contents.encoded[3]);

		// Newline after every 72 chars (18 * 4)
		if (++w % 18 == 0)
			fprintf(keyfile, "\n");
	}
	
	// Close mirror file
	fclose(keyfile);

	return 1;
}

/*
 * The keyfile_shuffle_string() function shuffles the contents of the
 * provided string pointer. The shuffle loop is repeated the number of
 * times specified in p.
 * 
 * Once the shuffle is complete, the same string pointer is returned.
 */
unsigned char *keyfile_shuffle_string(unsigned char *str, int p) {
	int i, s, sIndex, rIndex;
	unsigned char c1, c2;
	FILE *urandom;
	uint32_t randseed = 0;
	
	// Open the urandom resource
	if ((urandom = fopen("/dev/urandom", "r")) == NULL)
		return 0;
	
	sIndex = (rand() % GRID_SIZE * 4);
	rIndex = sIndex;
	c1 = str[rIndex];
	for (i = 0; i < p; ++i) {
		
		// (Re)seed our PRNG
		for (s = 24; s >= 0; s -= 8)
			randseed += ((uint32_t)fgetc(urandom) << s);
		srand(randseed);
		
		// rIndex can't equal sIndex. sIndex is reserved for the
		// placement of the last character shuffled.
		while ((rIndex = (rand() % (GRID_SIZE * 4))) == sIndex)
			;
		
		c2 = str[rIndex];
		str[rIndex] = c1;
		c1 = c2;
	}
	str[sIndex] = c1;
	
	// Close urandom resource
	fclose(urandom);
	
	return str;
}

/*
 * The keyfile_next_char() function is responsible for reading and decoding
 * the contents of the key file. It returns a single unsigned char cast
 * as an integer for each call.
 */
int keyfile_next_char(void) {
	static base64 contents = { .index = BASE64_DECODED_COUNT, .error = 0 };

	if (contents.index == BASE64_DECODED_COUNT) {
		contents.index = 0;

		while (contents.index < BASE64_ENCODED_COUNT) {
			contents.encoded[contents.index] = fgetc(keyFile);
			if (contents.encoded[contents.index] == EOF)
				return EOF;
			else if (contents.encoded[contents.index] == '\n')
				continue;
			else
				++contents.index;
		}

		contents = base64_decode(contents);
		if (contents.error)
			return EOF;
		else
			contents.index = 0;
	}

	return (int)contents.decoded[contents.index++];
}

/*
 * The keyfile_close() function closes the key file pointer if it has
 * been opened.
 */
void keyfile_close(void) {
	if (keyFile != NULL) {
		fclose(keyFile);
		keyFile = NULL;
	}
}
