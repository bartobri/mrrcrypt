Mirror Crypt
============

A command line encryption/decryption tool for ASCII text using a
spinning mirror field algorithm.

![Screen Cap](http://i.imgur.com/mh8efa2.gif)

Quick Examples
--------------

**Example #1** - Encrypting/Decrypting a short string.

**Example #2** - Encrypting/Decrypting the contents of a ASCII file.

**Example #3** - Encrypting/Decrypting the contents of a binary file using a base64 encoder.

**Example #4** - Dual key encryption.

About
-----

Mirror Crypt is a handy tool to easily encrypt/decrypt ASCII text using a
mirror field algorithm with rotating mirrors.

A traditional mirror field produces the same output character for the same
input character, not disimilar to a simple substitution cipher, making it
a weak form of encryption.

Mirror Crypt employes a method of spinning the mirrors clockwise each time
they are encountered in the encryption/decryption of a character. This produces
a different, highly unpredictable result for each input character, even
if they are the same.

Example:

Input | Output
----- | ------
Attack at Dawn | w]M_0FSWAqMmD~
Mississippi | LWBZe8Bq<zo

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
