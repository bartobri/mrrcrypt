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
#include "modules/mirrorfile.h"

#define MIRROR_FILE_NAME "default"
#define MIRROR_FILE_PATH ".config/mirrorcrypt/"

// Static chars
static FILE *mirrorFile;

void mirrorfile_init(void) {
	mirrorFile = NULL;
}

int mirrorfile_open(char *homeDir, char *mirrorFileName) {
	int r = 1;
	char *mirrorFilePath = MIRROR_FILE_PATH;
	char *mirrorFilePathName;
	char *mirrorFileFullPathName;
	
	// Set mirror file name to default if NULL
	if (mirrorFileName == NULL)
		mirrorFileName = MIRROR_FILE_NAME;
	
	// Combine mirror file path and name in to one string.
	mirrorFilePathName = malloc(strlen(mirrorFilePath) + strlen(mirrorFileName) + 1);
	sprintf(mirrorFilePathName, "%s%s", mirrorFilePath, mirrorFileName);
	
	// Build mirror file path
	mirrorFileFullPathName = malloc(strlen(mirrorFilePathName) + strlen(homeDir) + 2);
	sprintf(mirrorFileFullPathName, "%s/%s", homeDir, mirrorFilePathName);

	if ((mirrorFile = fopen(mirrorFileFullPathName, "r")) == NULL)
		r = 0;
	
	free(mirrorFilePathName);
	free(mirrorFileFullPathName);
	
	return r;
}

int mirrorfile_create(char *homeDir, char *mirrorFileName, int width) {
	int i, r, c;
	struct stat sb;
	FILE *config;
	char *mirrorFilePath = MIRROR_FILE_PATH;
	char *mirrorFilePathName;
	char *mirrorFileFullPathName;
	char *shuffledChars;
	
	// Set mirror file name to default if NULL
	if (mirrorFileName == NULL)
		mirrorFileName = MIRROR_FILE_NAME;
	
	// Combine mirror file path and name in to one string.
	mirrorFilePathName = malloc(strlen(mirrorFilePath) + strlen(mirrorFileName) + 1);
	sprintf(mirrorFilePathName, "%s%s", mirrorFilePath, mirrorFileName);
	
	// Build mirror file path
	mirrorFileFullPathName = malloc(strlen(mirrorFilePathName) + strlen(homeDir) + 2);
	sprintf(mirrorFileFullPathName, "%s/%s", homeDir, mirrorFilePathName);

	// Check subdirs and create them if needed
	if (strchr(mirrorFileFullPathName, '/') != NULL) {
		for (i = 1; mirrorFileFullPathName[i] != '\0'; ++i) {
			if (mirrorFileFullPathName[i] == '/') {
				mirrorFileFullPathName[i] = '\0';
				if (stat(mirrorFileFullPathName, &sb) != 0)
					if (mkdir(mirrorFileFullPathName, 0700) == -1)
						return 0;
				mirrorFileFullPathName[i] = '/';
			}
		}
	}

	// Now lets open the mirror file
	if ((config = fopen(mirrorFileFullPathName, "w")) == NULL)
		return 0;

	// Seed my random number generator
	srand(time(NULL));

	// Write mirror data to file
	for (r = 0; r < width; ++r) {
		for (c = 0; c < width; ++c) {
			switch (rand() % 5) {
				case 1:
					fprintf(config, "/");
					break;
				case 2:
					fprintf(config, "\\");
					break;
				default:
					fprintf(config, " ");
					break;
			}
		}
	}
	
	// Shuffle and write character data to file
	shuffledChars = malloc(strlen(SUPPORTED_CHARS) + 1);
	strcpy(shuffledChars, SUPPORTED_CHARS);
	fprintf(config, "%s", mirrorfile_shuffle_string(shuffledChars, 1000));
	
	/*
	char *sChars = SUPPORTED_CHARS;
	for (i = 0; sChars[i] != '\0'; ++i) {
		int i2;
		int c = 0;
		for (i2 = 0; shuffledChars[i2] != '\0'; ++i2) {
			if (sChars[i] == shuffledChars[i2]) {
				++c;
			}
		}
		printf("%i count for %c\n", c, sChars[i]);
	}
	*/
	
	
	// Close mirror file
	fclose(config);

	return 1;
}

char *mirrorfile_shuffle_string(char *str, int p) {
	int i, sIndex, rIndex;
	char c1, c2;
	
	sIndex = (rand() % strlen(str));
	rIndex = sIndex;
	c1 = str[rIndex];
	for (i = 0; i < p; ++i) {
		
		// rIndex can't equal sIndex. sIndex is reserved for the
		// placement of the last character shuffled.
		while ((rIndex = (rand() % strlen(str))) == sIndex)
			;
		
		c2 = str[rIndex];
		str[rIndex] = c1;
		c1 = c2;
	}
	str[sIndex] = c1;
	
	return str;
}

int mirrorfile_next_char(void) {
	if (mirrorFile != NULL)
		return fgetc(mirrorFile);
	
	return -2;
}

void mirrorfile_close(void) {
	if (mirrorFile != NULL) {
		fclose(mirrorFile);
		mirrorFile = NULL;
	}
}
