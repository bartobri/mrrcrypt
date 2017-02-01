
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"

void show_stats(void);

static int data[GRID_SIZE*4];

int main(int argc, char *argv[]) {
	int ch, c = 0, o;
	int sample = 0;
	
	memset(data, 0, sizeof(data));
	
	// Check arguments
	while ((o = getopt(argc, argv, "s:")) != -1) {
		switch (o) {
			case 's':
				sample = atoi(optarg);
				break;
			case '?':
				if (isprint(optopt))
					fprintf (stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
		}
	}

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {
		(data[ch])++;
		++c;
		
		if (sample > 0 && sample == c) {
			show_stats();
			
			memset(data, 0, sizeof(data));
			c = 0;
		}
	}
	
	if (c) {
		show_stats();
	}

	return 0;
}

void show_stats(void) {
	int i;
	int high = 0, low = 0, diff = 0;
	float diffpct;
	
	for (i = 0; i < GRID_SIZE*4; ++i) {
		if (data[i] > high) {
			high = data[i];
		}
	}
	low = high;
	for (i = 0; i < GRID_SIZE*4; ++i) {
		if (data[i] < low) {
			low = data[i];
		}
	}
	diff = high - low;
	
	for (i = 0; i < GRID_SIZE*4; ++i) {
		printf("%i => %i\n", i, data[i]);
	}
	
	diffpct = ((float)diff / (float)high) * 100;
	
	printf("%i -> %i, diff: %i (%%%.2f)\n", low, high, diff, diffpct);
}
