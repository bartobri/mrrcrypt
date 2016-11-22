
#include <stdio.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[]) {
	int i, ch;
	int high = 0, low = 0, diff = 0;
	int data[GRID_SIZE*4];
	
	memset(data, 0, sizeof(data));

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {
		(data[ch])++;
	}
	
	for (i = 0; i < GRID_SIZE*4; ++i) {
		if (data[i] > high) {
			high = data[i];
		}
		if (data[i] < low) {
			low = data[i];
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
	printf("%i -> %i, diff: %i\n", low, high, diff);

	return 0;
}
