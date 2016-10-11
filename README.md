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

Mirror Crypt is a handy tool to easily encrypt/decrypt ASCII text using an
adaptive mirror field algorithm.

A traditional mirror field produces the same output character for the same
input character, not disimilar to a simple substitution cipher, making it
a weak form of encryption.

Mirror Crypt employes adaptive mirrors wjos orientation spins clockwise each time
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

COMING SOON

Config Dir
----------

TODO

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
