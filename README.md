Mirror Crypt
============

A command line encryption/decryption tool for ASCII text using an
adaptive mirror field algorithm.

*Mirror Crypt Visualizer*

![Screen Cap](http://i.imgur.com/mh8efa2.gif)

Quick Examples
--------------

**Encrypting/Decrypting a Short String**

```
$ printf "Attack at Dawn" | mrrcrypt
w]M_0FSWAqMmD~

$ printf "w]M_0FSWAqMmD~" | mrrcrypt
Attack at Dawn

```

**Encrypting/Decrypting the Contents of a ASCII File**

```
$ cat secrets.txt | mrrcrypt > secrets.encrypted

$ cat secrets.encrypted | mrrcrypt > secrets.txt
```

**Encrypting/Decrypting the Contents of a Binary File**

This method uses a base64 encoder to convert the binary file to ASCII before encrypting.

```
$ cat secret_photo.jpg | base64 | mrrcrypt > secret_photo.jpg.encrypted

$ cat secret_photo.jpg.encrypted | mrrcrypt | base64 --decode > secret_photo.jpg
```

**Dual Key Encryption/Decryption**

Suppose Bob and John want to encrypt a file such that they each hold a
separate key required for decryption. Nether one can decrypt the file without
the consent of the other. This is how they can accomplish this.

```
$ cat secrets.txt | mrrcrypt -a -m bobs_key | mrrcrypt -a -m johns_key > secrets.encrypted

$ cat secrets.encrypted | mrrcrypt -m johns_key | mrrcrypt -m bobs_key > secrets.txt
```

The `-m <name>` option specifies a mirror file to use, and the `-a`
option auto-creates it if it doesn't exist. After performing the first
command to create two new keys and encrypt the file, they each copy
their respective key files for themselves and delete the originals. Then
to decrypt, they each copy their key files to the original location and
perform the second command.

About
-----

Mirror Crypt is a command line encryption/decryption tool for ASCII text
using an adaptive mirror field algorithm.

A traditional mirror field encryption algorithm is nothing more than
elaborate form of a simple substitution cipher. For every identical
character of input, an identical character of output is produced. For
example, given an input string of "Mississippi"
which contains many duplicate characters, the output will also contain
duplicate characters in the same positions.

"Mississippi" -> Substitution Cypher (or Mirror Field) -> "jTaaTaaTxxT"

This makes for weak, easily defeatable encryption.

Mirror Crypt employs an *adaptive* mirror field algorithm. For each
character of input, mirrors in the field will change orientation,
producing different output character for each identical input character.

Since the placement and orientation of all the mirrors in the field serves
as a unique private key, each character of input is essentially given
it's own unique private key, which can only be derived with the private
key of the character preceeding it.

The result is a highly unpredictable, seemingly random output.

Example:

Input | Output
----- | ------
Mississippi | LWBZe8Bq<zo
Attack at Dawn | w]M_0FSWAqMmD~

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

COMING SOON

Config Dir
----------

TODO

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
