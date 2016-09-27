// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

// Static chars
static FILE *mirrorFile;

int mirrorfile_open(char *filePath) {

	if ((mirrorFile = fopen(filePath, "r")) == NULL)
		return 0;
	
	return 1;
}

int mirrorfile_create(char *file, int width) {
	int i, r, c;
	struct stat sb;
	FILE *config;

	// Check subdirs and create them if needed
	if (strchr(file, '/') != NULL) {
		for (i = 1; file[i] != '\0'; ++i) {
			if (file[i] == '/') {
				file[i] = '\0';
				if (stat(file, &sb) != 0)
					if (mkdir(file, 0700) == -1)
						return 0;
				file[i] = '/';
			}
		}
	}

	// Now lets open the mirror file
	if ((config = fopen(file, "w")) == NULL)
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
	return fgetc(mirrorFile);
}

void mirrorfile_close(void) {
	if (mirrorFile != NULL)
		fclose(mirrorFile);
}
