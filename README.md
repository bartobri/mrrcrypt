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

About MrrCrypt
--------------

MrrCrypt is a command-line tool for encryption/decryption. It
processes data through a 64x64-byte adaptive mirror field to produce a highly
unpredictable pattern of bits as output, the results of which can only be
reversed by the same mirror field permutation that produced it. The particular mirror
field permutation used for encryption therefore also serves as the key for decryption.

There are over 1.94*10^1954 possible key permutations, countless times more
than the number of atoms beleived to be on planet earth.

The first time you run `mrrcrypt`, a new randomized key is created for you.
This is your default key. mrrcrypt will use this key when no other
is specified. More keys can be generated and used as needed. See **Usage**
for more info.

**What is an adaptive mirror field?**

TODO

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
$ git clone https://github.com/bartobri/mrrcrypt.git
$ cd mrrcrypt
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

Pipe data to the `mrrcrypt` executable and it will produce encrypted
output using the specified key, or using the default key if not specified.

The keys also work in reverse. Pipe encrypted data to the `mrrcrypt`
executable and it will produce decrypted output. The same key used to
encrypt the data must be specified.

**Basic Encryption**

```
# Encrypt a String
$ echo "This is my secret" | mrrcrypt

# Encrypt a File (Text and Binary)
$ cat secret.txt | mrrcrypt
$ cat secret_photo.jpg | mrrcrypt

# Store the Encrypted Output to a File
$ cat secret.txt | mrrcrypt > secret.txt.encrypted
$ cat secret_photo.jpg | mrrcrypt > secret_photo.jpg.encrypted
```

**Basic Decryption**

```
# Decrypt a File
$ cat secrets.txt.encrypted | mrrcrypt

# Redirect Decrypted Output to a File
$ cat secrets.txt.encrypted | mrrcrypt > secrets.txt
$ cat secret_photo.jpg.encrypted | mrrcrypt > secret_photo.jpg
```

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
