// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See LICENSE for more details.

#ifndef MAIN_H
#define MAIN_H 1

/*
 * Program version number. Enumerated when the -v command option is used.
 */
#define VERSION                "0.1.0"

/*
 * The DEFAULT_KEY_NAME is the name of the default key that myycrypt will
 * use when no key name is specified by the use of the -k command option.
 */
#define DEFAULT_KEY_NAME       "default"

/*
 * The DEFAULT_KEY_PATH is the relative path, from the user's HOME directory,
 * to the directory that contains the key files. Note that if you are using
 * mrrcrypt with a user that has no home directory, you can use the -k
 * command option to specify an absolute path for a key file.
 */
#define DEFAULT_KEY_PATH       ".config/mrrcrypt/"

/*
 * The GRID_SIZE sets the size of the grid that mrrcrypt expects to find
 * in the key data. Do not ever change this unless you know what you are
 * doing.
 */
#define GRID_SIZE              64

#endif
