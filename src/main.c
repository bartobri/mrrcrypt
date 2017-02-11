// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <ctype.h>

#include "main.h"
#include "modules/keyfile.h"
#include "modules/mirrorfield.h"
#include "modules/base64.h"

// Function prototypes
void main_shutdown(const char *);

/*
 * The main function initializes the modules, checks arguments,
 * validates the key, and reads from STDIN 8 bits at a time. Each 8-bit
 * character is processed through the encryption algorithm and the result
 * is printed to STDOUT.
 */
int main(int argc, char *argv[]) {
	int o, ch;
	int autoCreate       = 0;
	int debug            = 0;
	char *version        = VERSION;
	char *keyFileName    = DEFAULT_KEY_NAME;
	
	// Run module init functions
	keyfile_init();
	mirrorfield_init();

	// Check arguments
	while ((o = getopt(argc, argv, "ak:vd:")) != -1) {
		switch (o) {
			case 'a':
				autoCreate = 1;
				break;
			case 'k':
				keyFileName = optarg;
				break;
			case 'v':
				printf("mrrcrypt version %s\n", version);
				return 0;
			case 'd':
				debug = atoi(optarg);
				break;
			case '?':
				if (isprint(optopt))
					fprintf (stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
				main_shutdown("Invalid command option(s).");
		}
	}
	
	// Turn on autoCreate flag for default key file
	if (strcmp(DEFAULT_KEY_NAME, keyFileName) == 0)
		autoCreate = 1;
	
	// Open key file
	if (keyfile_open(keyFileName, autoCreate) == 0) {
		if (autoCreate)
			main_shutdown("Could not auto-create key file. Check permissions.");
		else
			main_shutdown("Key file not found. Use -a to auto-create.");
	}

	// Read key file and build mirror field
	while ((ch = keyfile_next_char()) != EOF)
		if ((mirrorfield_set((unsigned char)ch)) == 0)
			break;

	// Close key file
	keyfile_close();
	
	// Validate mirror field contents
	if (mirrorfield_validate() == 0)
		main_shutdown("Key file error. Invalid content.");
	
	// Create grid links
	mirrorfield_link();

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {
		
		// Print cyphertext
		putchar(mirrorfield_crypt_char((unsigned char)ch, debug));
	}

	return 0;
}

/*
 * The main_shutdown() function ensures all open file descriptors are closed
 * cleanly before printing the shutdown message and exiting the program.
 */
void main_shutdown(const char *errmsg) {
	
	// Log a shutdown message
	fprintf (stderr, "Shutting down. Reason: %s\n", errmsg);
	
	// Close mirror file (if open)
	keyfile_close();
	
	// Shutdown
	exit(1);
}
