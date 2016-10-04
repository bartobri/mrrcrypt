Mirror Crypt
============

A mirror field encryption implementation as a command line tool.

SCREEN GRAB HERE

About
-----

Mirror Crypt is a handy tool to easily encrypt/decrypt ASCII text using a
mirror field traversing algorithm.

Mirror Crypt improves on the traditional mirror field traversing algorithm
by implementing a "mirror spin" each time a mirror is encountered in the
encryption/decryption process. This allows the same letter to have a
different encrypted result each time, because the mirror field is changed
with each character traversal.

Download and Install
--------------------

In order to download and build this program, you will need to have git,
gcc, and make installed. Install them from your package manager if not
already installed.

```
$ which make
/usr/bin/make

$ which gcc
/usr/bin/gcc

$ which git
/usr/bin/git
```
Download and Build:
```
$ git clone https://github.com/bartobri/mirror-crypt.git
$ cd mirror-crypt
$ make
```

Install:
```
$ sudo make install
```

Uninstall:
```
$ sudo make uninstall
```

Usage
-----

The first time you use `mirrorcrypt` you will need to use the `-a` option
to auto-create your default mirror field file, which essentially serves
as your personal private key.

```
$ printf "hello world" | mirrorcrypt -a
ggYt~Vy[mYg
```

From that point on you no longer need the `-a` option.

```
$ printf "hello world" | mirrorcrypt
ggYt~Vy[mYg
```

Config Dir
----------

TODO

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
