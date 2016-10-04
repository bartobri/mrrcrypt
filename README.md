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
Next download and build the application:
```
git clone https://github.com/bartobri/mirror-crypt.git
cd mirror-crypt
make
```

Install the application:
```
TODO - Need install process
```

Usage
-----

TODO

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
