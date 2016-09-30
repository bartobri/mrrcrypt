// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

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
			switch (rand() % 10) {
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
		fprintf(config, "\n");
	}
	fclose(config);

	return 1;
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
