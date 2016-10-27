MrrCrypt
========

A command line encryption/decryption tool using an adaptive mirror field algorithm.

Quick Examples
--------------

**Encrypting/Decrypting a Short String**

```
# Encrypt
$ printf "Attack at Dawn" | mrrcrypt > string.encrypted

# Decrypt
$ cat string.encrypted | mrrcrypt
Attack at Dawn

```

**Encrypting/Decrypting the Contents of a File**

```
# Encrypt
$ cat secret.jpg | mrrcrypt > secret.jpg.encrypted

# Decrypt
$ cat secret.jpg.encrypted | mrrcrypt > secret.jpg
```

**Dual Key Encryption/Decryption**

Suppose Bob and John want to encrypt a file so that they each hold a
distinct key required for decryption. Here is how they can accomplish this.

```
# Encrypt
$ cat secret.txt | mrrcrypt -a -k bobs_key | mrrcrypt -a -k johns_key > secret.txt.encrypted

# Decrypt
$ cat secret.txt.encrypted | mrrcrypt -k johns_key | mrrcrypt -k bobs_key > secret.txt
```

The `-k <name>` option specifies a key file to use, and the `-a`
option auto-creates it if it doesn't exist. After performing the first
command to create two new keys and encrypt the file, they each copy
their respective key files for themselves and delete the originals. Then
to decrypt, they each copy their key files to the original location and
perform the second command.

About
-----

MrrCrypt is a command line encryption/decryption tool using an **adaptive
mirror field** algorithm.

What is a mirror field?

A traditional (non-adaptive) mirror field is a construct whereby you are
given a square grid, and inside the grid is a random assortment of mirrors
orientated diagonlly. Around the outside of the grid are the characters to
be encrypted/decrypted. It looks like this:

```
   A B C D E F G H
  -----------------
Q : : : :\: : : : : I
  -----------------
R :\: : : : : : : : J
  -----------------
S : : : : : : :/: : K
  -----------------
T : : :\: : : : : : J
  -----------------
U : : : : :\: : : : M
  -----------------
V : : : : : : : :\: N
  -----------------
W : :/: : : : : : : O
  -----------------
X : : : : : :/: : : P
  -----------------
   Y Z 1 2 3 4 5 6
```


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
