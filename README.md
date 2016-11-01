MrrCrypt
========

A command line encryption/decryption tool using an adaptive mirror field algorithm.

**Table of Contents**

1. [Quick Examples](#quick-examples)
2. [About MrrCrypt](#about-mrrcrypt)
3. [Download and Install](#download-and-install)
4. [Usage](#usage)
5. [License](#license)

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
processes data through an adaptive mirror field algorithm to produce a highly
unpredictable pattern of bits as output, the results of which can only be
reversed by the same mirror field permutation that produced it. The particular mirror
field permutation used for encryption therefore also serves as the key for decryption.

There are over 1.94*10^1954 possible key permutations, countless times more
than the number of atoms beleived to be on planet earth.

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

`mrrcrypt` processes data from stdin and writes the results to stdout.
Use I/O redirection for generating input and capturing output.

**Basic Encryption**

```
# Encrypt a String
$ echo "This is my secret" | mrrcrypt > secret_string.encrypted

# Encrypting a file
$ cat secret_photo.jpg | mrrcrypt > secret_photo.jpg.encrypted
```

**Basic Decryption**

```
# Decrypt a File
$ cat secret_string.encrypted | mrrcrypt
This is my secret

# Capture Output to a File
$ cat secret_photo.jpg.encrypted | mrrcrypt > secret_photo.jpg
```

**Command Line Options**

-k key_name

Specify an alternate key to use. "key_name" is required.

-a

Auto-create a new key if the one specified by `-k key_name` does not exist.

```
# Create a new key called "bobs_key" for encryption
$ echo "This is my secret message" | mrrcrypt -a -k bobs_key > bobs_message.txt

# Specify the key "bobs_key" for decryption
$ cat bobs_message.txt | mrrcrypt -k bobs_key
This is my secret message
```

-d *ms*

Debug mode. This draws the mirror field and animates the decryption process
for debugging purposes. You will need a minimum temrinal size of 134x68.
"ms" specifies the time in milliseconds for each step through the mirror
field. Values around the mirror field perimeter are represented in hexidecimal.

**Key Management**

Keys are stored in `$HOME/.config/mrrcrypt/`

The first time you run `mrrcrypt` it will automatically generate a unique
default key. The default key will be used when no key is specified.

You may wish to share your default key (or any other key) with others so
they can decrypt your messages. Keys that are shared by other individuals
must be copied to the above directory so they can be used with the
`-k key_name` option.

Note that "key_name" coresponds to the name of the key file. If John
encrypts messages using his "default" key file and gives a copy to Bob so he
can decrypt them, Bob should rename the file to "johns_key" so he does not
overwrite his own default key file. Then Bob can use `-k johns_key` when
decrypting data sent from John.

Keys are stored in Base64 encoded format.

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
