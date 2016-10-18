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

int main(int argc, char *argv[]) {
	int o, i, ch;
	int autoCreate       = 0;
	int debug            = 0;
	int debugSpeed       = 0;
	char *version        = VERSION;
	char *keyFileName    = DEFAULT_KEY_NAME;
	
	// Run module init functions
	keyfile_init();
	mirrorfield_init();
	
	// Validate supported character count is square
	if (strlen(SUPPORTED_CHARS) % 4 != 0)
		main_shutdown("Invalid character set. Character count must be evenly divisible by 4.");

	// Validate supported character count is compatible with grid width
	if (strlen(SUPPORTED_CHARS) / 4 != GRID_SIZE)
		main_shutdown("Invalid character set. Character count does not match grid width.");

	// Check arguments
	while ((o = getopt(argc, argv, "ak:vds:")) != -1) {
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
				debug = 100;
				break;
			case 's':
				debugSpeed = atoi(optarg);
				break;
			case '?':
				if (isprint(optopt))
					fprintf (stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
				main_shutdown("Invalid command option(s).");
		}
	}
	
	// Set value of debug to debug speed if both options are used
	if (debug && debugSpeed)
		debug = debugSpeed;
	
	// Turn on autoCreate flag for default key file
	if (strcmp(DEFAULT_KEY_NAME, keyFileName) == 0)
		autoCreate = 1;
	
	// Open key file
	if (keyfile_open(keyFileName, autoCreate) == 0) {
		if (autoCreate)
			main_shutdown("Could not auto-create key file. Make sure $HOME is set to a writable directory.");
		else
			main_shutdown("Key file not found. Use -a to auto-create.");
	}

	// Read key file and build mirror field
	for (i = 0; (ch = keyfile_next_char()) != EOF; ++i)
		if ((mirrorfield_set(i, ch)) == 0)
			break;

	// Close key file
	keyfile_close();
	
	// Validate mirror field contents
	if (mirrorfield_validate() == 0)
		main_shutdown("Invalid key file. Invalid content.");

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {

		// If character not supported, just print it and continue the loop
		if (ch == '\0' || strchr(SUPPORTED_CHARS, ch) == NULL) {
			putchar(ch);
			continue;
		}
		
		// Print encrypted/decrypted char
		putchar(mirrorfield_crypt_char(ch, debug));
		
		// Rotate perimeter chars
		mirrorfield_rotate();
	}

	return 0;
}

void main_shutdown(const char *errmsg) {
	
	// Log a shutdown message
	printf("Shutting down. Reason: %s\n", errmsg);
	
	// Close mirror file (if open)
	keyfile_close();
	
	// Shutdown
	exit(1);
}
