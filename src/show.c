
#include <stdio.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[]) {
	int i, ch;
	int data[GRID_SIZE*4];
	
	memset(data, 0, sizeof(data));

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {
		(data[ch])++;
	}
	
	for (i = 0; i < GRID_SIZE*4; ++i) {
		printf("%i => %i\n", i, data[i]);
	}

	return 0;
}
