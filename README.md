MrrCrypt
========

A command line encryption/decryption tool that uses an adaptive mirror field algorithm.

*MrrCrypt encrypting a message in debug mode:*

![MrrCrypt's Adaptve Mirror Field](http://i.imgur.com/4GRcmzF.gifv)

**Table of Contents**

1. [Quick Examples](#quick-examples)
2. [About MrrCrypt](#about-mrrcrypt)
3. [Download and Install](#download-and-install)
4. [Usage](#usage)
5. [Key Management](#key-management)
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
distinct key required for decryption. MrrCrypt can easily accomplish this.

```
# Encrypt
$ cat secret.txt | mrrcrypt -a -k bobs_key | mrrcrypt -a -k johns_key > secret.txt.encrypted

# Decrypt
$ cat secret.txt.encrypted | mrrcrypt -k johns_key | mrrcrypt -k bobs_key > secret.txt
```

The `-k <name>` option specifies a key file to use, and the `-a`
option auto-creates it if it doesn't exist. After performing the first
command to create two new keys and encrypt the file, they each copy
their respective key files for themselves (see [Key Management](#key-management))
and delete the originals. To decrypt, they each copy their key files back
to the original location and perform the second command.

About MrrCrypt
--------------

MrrCrypt is a command-line tool for encryption/decryption. It
processes data through an adaptive mirror field algorithm to produce a highly
unpredictable pattern of bits as output. The unique mirror field permutation
used for encryption therefore also serves as the key for decryption.

**What is an adaptive mirror field?**

See [ADAPTIVE_MIRROR_FIELD.md](ADAPTIVE_MIRROR_FIELD.md).

Download and Install
--------------------

In order to download and build this program, you will need to have `git`,
`gcc`, and `make` installed. Install them from your package manager if not
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

`-k key_name`

Specify a key to use. `key_name` is required. Note that when this option
is not used, MrrCrypt uses the default key. See [Key Management](#key-management)
for more info.

`-a`

Auto-create a new key if the one specified by `-k key_name` does not exist.

`-d ms`

Debug mode. This draws the mirror field and animates the decryption process
for debugging purposes. You will need a minimum temrinal size of 134x68.
"ms" specifies the time in milliseconds for each step through the mirror
field. Values around the mirror field perimeter are shown in hexidecimal.
A total of 256 values are represented in the 64x64 grid, one value for
every possible arrangement of bits in a single byte.

![MrrCrypt's Mirror Field](http://i.imgur.com/cXrVwqe.jpg)

Key Management
--------------

Keys are stored in `$HOME/.config/mrrcrypt/`

The first time you run `mrrcrypt` it will automatically generate a unique
default key. The default key will be used when no key is specified.

You may wish to share your default key (or any other key) with others so
they can decrypt your messages. Keys that are shared by other individuals
must be copied to the above directory so they can be used with the
`-k key_name` option.

Note that "key_name" coresponds to the name of the key file. If John
encrypts messages using his "default" key file, and gives a copy to Bob so he
can decrypt them, Bob should rename the file to "johns_key" so he does not
overwrite his own default key file. Then Bob can then use `-k johns_key` when
decrypting data sent from John.

Keys are stored in Base64 encoded format.

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the the
MIT License (MIT). See [LICENSE](LICENSE) for more details.
