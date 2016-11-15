Adaptive Mirror Field
=====================

To help explain what an adaptive mirror field is, let's
first explain what a typical, non-adaptive mirror field is.

Non-Adaptive Mirror Field
-------------------------

A **mirror field** is a square grid that contains randomly placed mirrors
in a diagonal orientation. Characters that can be encrypted are placed
around the perimeter of the mirror field. Encryption occurres by standing
on the character you wish to encrypt, looking straight into the field,
and observing the character that the mirrors reflect back.

Mirror Field Example:

```
                A B C D E
               -----------
              K: :/: :/: :F
               -----------
              L: : :\: :/:G
  Input        -----------       Output
 ------- ==>  M:\: : :/: :H  ==> -------
 "ABCDE"       -----------       "FKEMC"
              N:\:/: : : :I
               -----------
              O: : :/: :\:J
               -----------
                P Q R S T
```

In practice, using a typical mirror field is a weak form of encryption.
Given the same plaintext character as input, the same cyphertext character
is produced as output, creating a pattern and providing hints that an attacker
can use to derive the key.

```
                A B C D E
               -----------
              K: :/: :/: :F
               -----------
              L: : :\: :/:G
  Input        -----------       Output
 ------- ==>  M:\: : :/: :H  ==> -------
 "CCCCC"       -----------       "EEEEE"
              N:\:/: : : :I
               -----------
              O: : :/: :\:J
               -----------
                P Q R S T
```

Adaptive Mirror Field
---------------------

In an adaptive mirror field, the mirror orientations and position of the perimeter characters are dynamic.

For every character that is processed:

* The character's path through the mirror field reorients the mirrors it contacts, creating a unique mirror field permutation for the next character.
* The plaintext and cyphertext characters are moved to a new perimeter posion.
   
These two features eliminate identifiable patterns in the output, most importantly when given repeating characters as input.

MrrCrypt's Adaptive Mirror Field
--------------------------------

MrrCrypt encrypts data 1 byte at a time, so the mirror field was built to accomodate a 256 character alphabet.
It consists of a 32x32 square grid with two rows of perimeter characters.

![MrrCrypt's Adaptve Mirror Field](http://i.imgur.com/UzG0DFV.gif)
