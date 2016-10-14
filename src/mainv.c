// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include <ncurses.h>

#include "main.h"
#include "modules/keyfile.h"
#include "modules/mirrorfield.h"
#include "modules/visitedmirrors.h"

#define MIRROR_NONE      32
#define MIRROR_FORWARD   47
#define MIRROR_BACKWARD  92
#define MIRROR_STRAIGHT  45
#define DIR_UP           1
#define DIR_DOWN         2
#define DIR_LEFT         3
#define DIR_RIGHT        4

// For Ncurses
#define DEFAULT_STEP_MS              100
#define INPUT_TITLE_WINDOW_ROWS      1
#define INPUT_WINDOW_ROWS            3
#define GRID_WINDOW_ROWS             GRID_SIZE + 6
#define RESULT_WINDOW_ROWS           3
#define MIN_ROWS                     INPUT_TITLE_WINDOW_ROWS + INPUT_WINDOW_ROWS + GRID_WINDOW_ROWS + RESULT_WINDOW_ROWS
#define MIN_COLS                     80

// Function prototypes
void main_draw_field(WINDOW *, int);
void main_shutdown(const char *);

int main(int argc, char *argv[]) {
	int o, r, c, ch, i;
	int autoCreate       = 0;
	char *version        = VERSION;
	char *keyFileName = DEFAULT_KEY_NAME;
	
	// ncurses vars
	int termSizeRows, termSizeCols;
	char *wInputHeading   = " Input ";
	char *wGridHeading    = " Mirror Field ";
	char *wResultHeading  = " Output ";
	WINDOW *wGrid;
	WINDOW *wResult;
	WINDOW *wInput;
	WINDOW *wInputTitle;
	struct timespec ts;
	ts.tv_sec = DEFAULT_STEP_MS / 1000;
    ts.tv_nsec = (DEFAULT_STEP_MS % 1000) * 1000000;
	
	// Run module init functions
	keyfile_init();
	mirrorfield_init();
	visitedmirrors_init();
	
	// Validate supported character count is square
	if (strlen(SUPPORTED_CHARS) % 4 != 0)
		main_shutdown("Invalid character set. Character count must be evenly divisible by 4.");

	// Validate supported character count is compatible with grid width
	if (strlen(SUPPORTED_CHARS) / 4 != GRID_SIZE)
		main_shutdown("Invalid character set. Character count does not match grid width.");

	// Check arguments
	while ((o = getopt(argc, argv, "ak:s:v")) != -1) {
		switch (o) {
			case 'a':
				autoCreate = 1;
				break;
			case 'k':
				keyFileName = optarg;
				break;
			case 's':
				ts.tv_sec = atoi(optarg) / 1000;
				ts.tv_nsec = (atoi(optarg) % 1000) * 1000000;
				break;
			case 'v':
				printf("mirrorcrypt version %s\n", version);
				return 0;
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
	
	// Open mirror file
	if (keyfile_open(keyFileName, autoCreate) == 0) {
		if (autoCreate)
			main_shutdown("Could not auto-create key file. Make sure $HOME is set to a writable directory.");
		else
			main_shutdown("Key file not found. Use -a to auto-create.");
	}

	// Read keyfile and build mirror field
	for (i = 0; (ch = keyfile_next_char()) != EOF; ++i)
		if ((mirrorfield_set(i, ch)) == 0)
			break;

	// Close mirror file
	keyfile_close();
	
	// Validate mirror field contents
	if (mirrorfield_validate() == 0)
		main_shutdown("Invalid key file. Invalid content.");
	
	// If we're here, all checked have passed. Let's start curses mode
	// and set a few things.
	initscr();
	cbreak();
	noecho();
	
	// Get terminal window size
	getmaxyx(stdscr, termSizeRows, termSizeCols);
	
	// Require minimum terminal size
	if (termSizeRows < MIN_ROWS || termSizeCols < MIN_COLS) {
		endwin();
		printf("Minimum terminal size must be %ix%i\n", MIN_COLS, MIN_ROWS);
		main_shutdown("Terminal window too small.");
	}
	
	// Create input title window
	wInputTitle = newwin(INPUT_TITLE_WINDOW_ROWS, termSizeCols, 0, 0);
	wmove(wInputTitle, 0, 0);
	whline(wInputTitle, ACS_CKBOARD, termSizeCols);
	mvwaddstr(wInputTitle, 0, (termSizeCols - strlen(wInputHeading)) / 2, wInputHeading);
	wrefresh(wInputTitle);
	
	// Create input window
	wInput = newwin(INPUT_WINDOW_ROWS, termSizeCols, INPUT_TITLE_WINDOW_ROWS, 0);
	scrollok(wInput, true);
	wrefresh(wInput);
	
	// Create grid window
	wGrid = newwin(GRID_WINDOW_ROWS, termSizeCols, INPUT_WINDOW_ROWS + INPUT_TITLE_WINDOW_ROWS, 0);
	wborder(wGrid, ' ', ' ', ACS_CKBOARD, ACS_CKBOARD,ACS_CKBOARD,ACS_CKBOARD,ACS_CKBOARD,ACS_CKBOARD);
	mvwaddstr(wGrid, 0, (termSizeCols - strlen(wGridHeading)) / 2, wGridHeading);
	mvwaddstr(wGrid, GRID_WINDOW_ROWS - 1, (termSizeCols - strlen(wResultHeading)) / 2, wResultHeading);
	wrefresh(wGrid);
	
	// Create result window
	wResult = newwin(termSizeRows - 1 - (GRID_WINDOW_ROWS + INPUT_WINDOW_ROWS), termSizeCols, GRID_WINDOW_ROWS + INPUT_WINDOW_ROWS + INPUT_TITLE_WINDOW_ROWS, 0);
	scrollok(wResult, true);
	wrefresh(wResult);
	
	main_draw_field(wGrid, termSizeCols);
	
	// Position cursor in input/result window
	wmove(wResult, 1, 0);
	wmove(wInput, 1, 0);

	// Loop over input one char at a time and encrypt
	while ((ch = getchar()) != EOF) {

		waddch(wInput, ch);
		wrefresh(wInput);

		int direction = 0;
		char ech = 0;

		// If character not supported, just print it and continue the loop
		if (ch == '\0' || index(SUPPORTED_CHARS, ch) == NULL) {
			waddch(wResult, ch);
			wrefresh(wResult);
			continue;
		}

		// Determining starting row/col and starting direction
		for (r = 0; r < GRID_SIZE; ++r) {
			for (c = 0; c < GRID_SIZE; ++c) {
				if (mirrorfield_get_charup(r, c) == ch)
					direction = DIR_DOWN;
				else if (mirrorfield_get_charleft(r, c) == ch)
					direction = DIR_RIGHT;
				else if (mirrorfield_get_charright(r, c) == ch)
					direction = DIR_LEFT;
				else if (mirrorfield_get_chardown(r, c) == ch)
					direction = DIR_UP;
				
				if (direction)
					break;
			}
			if (direction)
				break;
		}
		
		// Show cursor over starting char
		if (direction == DIR_DOWN)
			wmove(wGrid, r + 2, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 2);
		else if (direction == DIR_LEFT)
			wmove(wGrid, r + 3, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 4);
		else if (direction == DIR_RIGHT)
			wmove(wGrid, r + 3, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 0);
		else if (direction == DIR_UP)
			wmove(wGrid, r + 4, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 2);
		wrefresh(wGrid);
		nanosleep(&ts, NULL);
		nanosleep(&ts, NULL);
		nanosleep(&ts, NULL);
		nanosleep(&ts, NULL);
		
		// Clear visited points
		visitedmirrors_clear();

		// Traverse through the grid
		while (ech == 0) {
			
			// If we hit a mirror that we've already been to, un-rotate it.
			// This is necessary to preserve the ability to reverse the encryption.
			// We can not rotate a mirror more than once per character.
			if (mirrorfield_get_type(r, c) != MIRROR_NONE) {
				if (visitedmirrors_exists(r, c)) {
					if (mirrorfield_get_type(r, c) == MIRROR_FORWARD) {
						mirrorfield_set_type(r, c, MIRROR_BACKWARD);
					} else if (mirrorfield_get_type(r, c) == MIRROR_BACKWARD) {
						mirrorfield_set_type(r, c, MIRROR_STRAIGHT);
					} else if (mirrorfield_get_type(r, c) == MIRROR_STRAIGHT) {
						mirrorfield_set_type(r, c, MIRROR_FORWARD);
					}
				} else {
					visitedmirrors_add(r, c);
				}
			}

			// Forward mirror / Change direction and rotate
			if (mirrorfield_get_type(r, c) == MIRROR_FORWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_LEFT;
				else if (direction == DIR_LEFT)
					direction = DIR_DOWN;
				else if (direction == DIR_RIGHT)
					direction = DIR_UP;
				else if (direction == DIR_UP)
					direction = DIR_RIGHT;
				
				// Spin mirror
				mirrorfield_set_type(r, c, MIRROR_STRAIGHT);
				
				// Show spun mirror in grid window
				wmove(wGrid, r + 3, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 2);
				wrefresh(wGrid);
				nanosleep(&ts, NULL);
				waddch(wGrid, '-');
			}
			
			// Straight mirror - Keep same direction, just rotate
			else if (mirrorfield_get_type(r, c) == MIRROR_STRAIGHT) {

				// Spin mirror
				mirrorfield_set_type(r, c, MIRROR_BACKWARD);
				
				// Show spun mirror in grid window
				wmove(wGrid, r + 3, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 2);
				wrefresh(wGrid);
				nanosleep(&ts, NULL);
				waddch(wGrid, '\\');
			}

			// Backward mirror \ Change direction and rotate
			else if (mirrorfield_get_type(r, c) == MIRROR_BACKWARD) {
				if (direction == DIR_DOWN)
					direction = DIR_RIGHT;
				else if (direction == DIR_LEFT)
					direction = DIR_UP;
				else if (direction == DIR_RIGHT)
					direction = DIR_DOWN;
				else if (direction == DIR_UP)
					direction = DIR_LEFT;
				
				// Spin mirror
				mirrorfield_set_type(r, c, MIRROR_FORWARD);

				// Show spun mirror in grid window
				wmove(wGrid, r + 3, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 2);
				wrefresh(wGrid);
				nanosleep(&ts, NULL);
				waddch(wGrid, '/');
			}

			// Advance position
			if (direction == DIR_DOWN)
				++r;
			else if (direction == DIR_LEFT)
				--c;
			else if (direction == DIR_RIGHT)
				++c;
			else if (direction == DIR_UP)
				--r;

			// Check if our position is out of grid bounds. That means we found our char.
			if (r < 0)
				ech = mirrorfield_get_charup(0, c);
			else if (r >= GRID_SIZE)
				ech = mirrorfield_get_chardown(GRID_SIZE - 1, c);
			else if (c < 0)
				ech = mirrorfield_get_charleft(r, 0);
			else if (c >= GRID_SIZE)
				ech = mirrorfield_get_charright(r, GRID_SIZE - 1);
			
			// Highlight position in grid window and sleep
			wmove(wGrid, r + 3, ((termSizeCols - (GRID_SIZE * 2)) / 2) + (c*2) + 2);
			wrefresh(wGrid);
			nanosleep(&ts, NULL);

		}
		waddch(wResult, ech);
		wrefresh(wResult);
		
		// Rotate Mirrors
		mirrorfield_rotate();
		
		// Redraw field
		main_draw_field(wGrid, termSizeCols);
	}
	
	// Set input to keyboard
	if (!isatty(STDIN_FILENO))
		freopen ("/dev/tty", "r", stdin);
	
	// Press key to continue
	flushinp();
	wgetch(wResult);
	
	// End curses mode
	endwin();

	return 0;
}

void main_draw_field(WINDOW *wGrid, int termSizeCols) {
	int r, c;
	char ch;
	
	for (r = -1; r <= GRID_SIZE; ++r) {

		wmove(wGrid, r + 3, (termSizeCols - (GRID_SIZE * 2)) / 2);

		for (c = -1; c <= GRID_SIZE; ++c) {
			
			if (r == -1 && c == -1)                     // Upper left corner
				waddch(wGrid, ' ');
			else if (r == -1 && c == GRID_SIZE)         // Upper right corner
				waddch(wGrid, ' ');
			else if (r == GRID_SIZE && c == -1)         // Lower left corner
				waddch(wGrid, ' ');
			else if (r == GRID_SIZE && c == GRID_SIZE)  // Lower right corner
				waddch(wGrid, ' ');
			else if (r == -1) {                           // Top chars
				ch = mirrorfield_get_charup(r + 1, c);
				if (isspace(ch))
					waddch(wGrid, ' ');
				else
					waddch(wGrid, ch);
			} else if (c == GRID_SIZE) {                   // Right chars
				ch = mirrorfield_get_charright(r, c - 1);
				if (isspace(ch))
					waddch(wGrid, ' ');
				else
					waddch(wGrid, ch);
			} else if (r == GRID_SIZE) {                  // Bottom chars
				ch = mirrorfield_get_chardown(r - 1, c);
				if (isspace(ch))
					waddch(wGrid, ' ');
				else
					waddch(wGrid, ch);
			} else if (c == -1) {                         // Left chars
				ch = mirrorfield_get_charleft(r, c + 1);
				if (isspace(ch))
					waddch(wGrid, ' ');
				else
					waddch(wGrid, ch);
			} else if (mirrorfield_get_type(r, c) == MIRROR_FORWARD)
				waddch(wGrid, '/');
			else if (mirrorfield_get_type(r, c) == MIRROR_BACKWARD)
				waddch(wGrid, '\\');
			else if (mirrorfield_get_type(r, c) == MIRROR_STRAIGHT)
				waddch(wGrid, '-');
			else
				waddch(wGrid, ' ');
			
			waddch(wGrid, ' ');

		}
	}
	wrefresh(wGrid);
}

void main_shutdown(const char *errmsg) {
	
	// Log a shutdown message
	printf("Shutting down. Reason: %s\n", errmsg);
	
	// Close mirror file (if open)
	keyfile_close();
	
	// Shutdown
	exit(1);
}
