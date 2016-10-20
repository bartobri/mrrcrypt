// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "main.h"
#include "modules/keyfile.h"
#include "modules/base64.h"

#define MIRROR_DENSITY   20

// Static chars
static FILE *keyFile;

void keyfile_init(void) {
	keyFile = NULL;
}

int keyfile_open(char *keyFileName, int autoCreate) {
	char *homeDir = getenv("HOME");
	char *keyFilePath = DEFAULT_KEY_PATH;
	char *keyFilePathName;
	char *keyFileFullPathName;
	
	// Return if we don't have a home directory
	if (homeDir == NULL)
		return 0;
	
	// Combine mirror file path and name in to one string.
	keyFilePathName = malloc(strlen(keyFilePath) + strlen(keyFileName) + 1);
	sprintf(keyFilePathName, "%s%s", keyFilePath, keyFileName);
	
	// Build mirror file path
	keyFileFullPathName = malloc(strlen(keyFilePathName) + strlen(homeDir) + 2);
	sprintf(keyFileFullPathName, "%s/%s", homeDir, keyFilePathName);

	while ((keyFile = fopen(keyFileFullPathName, "r")) == NULL) {
		if (autoCreate) {
			if (keyfile_create(keyFileFullPathName)) {
				continue;
			} 
		} 
		break;
	}
	
	free(keyFilePathName);
	free(keyFileFullPathName);
	
	if (keyFile == NULL)
		return 0;
	
	return 1;
}

int keyfile_create(char *keyFileFullPathName) {
	int i, r, c;
	int width = GRID_SIZE;
	struct stat sb;
	FILE *config;
	unsigned char perimeterChars[GRID_SIZE * 4];

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

	// Now lets open the mirror file
	if ((config = fopen(keyFileFullPathName, "w")) == NULL)
		return 0;

	// Seed my random number generator
	srand(time(NULL));

	// Write mirror data to file
	for (r = 0; r < width; ++r) {
		for (c = 0; c < width; ++c) {
			switch (rand() % MIRROR_DENSITY) {
				case 1:
					fprintf(config, base64_encode_char('/', B64_NOFORCE));
					break;
				case 2:
					fprintf(config, base64_encode_char('\\', B64_NOFORCE));
					break;
				default:
					fprintf(config, base64_encode_char(' ', B64_NOFORCE));
					break;
			}
		}
	}
	
	// Generate perimeter characters
	for (i = 0; i < GRID_SIZE * 4; i++)
		perimeterChars[i] = i;
		
	// Shuffle perimeter characters
	keyfile_shuffle_string(perimeterChars, 2000);
	
	// Encode perimeter chars to base64 and write to key file
	for (i = 0; i < GRID_SIZE * 4; ++i)
		if (i + 1 < GRID_SIZE * 4)
			fprintf(config, "%s", base64_encode_char(perimeterChars[i], B64_NOFORCE));
		else
			fprintf(config, "%s", base64_encode_char(perimeterChars[i], B64_FORCE));
	
	// Close mirror file
	fclose(config);

	return 1;
}

unsigned char *keyfile_shuffle_string(unsigned char *str, int p) {
	int i, sIndex, rIndex;
	unsigned char c1, c2;
	
	sIndex = (rand() % GRID_SIZE * 4);
	rIndex = sIndex;
	c1 = str[rIndex];
	for (i = 0; i < p; ++i) {
		
		// rIndex can't equal sIndex. sIndex is reserved for the
		// placement of the last character shuffled.
		while ((rIndex = (rand() % GRID_SIZE * 4)) == sIndex)
			;
		
		c2 = str[rIndex];
		str[rIndex] = c1;
		c1 = c2;
	}
	str[sIndex] = c1;
	
	return str;
}

int keyfile_next_char(void) {
	char ch;
	static int i = 3;
	static unsigned char *out = NULL;

	if (i == 3) {
		while (i-- >= 0) {
			if ((ch = fgetc(keyFile)) == EOF)
				return EOF;
			else
				out = base64_decode_char(ch);
		}
		i = 0;
	}

	return (int)out[i++];
}

void keyfile_close(void) {
	if (keyFile != NULL) {
		fclose(keyFile);
		keyFile = NULL;
	}
}
