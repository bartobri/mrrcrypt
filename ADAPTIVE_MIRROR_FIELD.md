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

In practice, using a typical mirror field is a weak form of encryption. The
same character of output is always produced for the same character of
input, creating a pattern of output characters that hint towards those used
for input.

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

Given a long enough encrypted message, one could deduce the unencrypted
message by examining patterns in the output.

Adaptive Mirror Field
---------------------

In an adaptive mirror field, the mirror orientations and position of the perimeter characters are dynamic.

For every character that is processed:

* The character's path through the mirror field reorients the mirrors it contacts, creating a unique mirror field permutation for the next character.
* The characters on the perimeter are rolled clockwise.
   
These two features eliminate identifiable patterns in the output.

![Adaptve Mirror Field](http://i.imgur.com/bKHZ9bl.gif)

MrrCrypt's Adaptive Mirror Field
--------------------------------

MrrCrypt's mirror field is much larger than the examples above. It is 64x64, the size needed to accommodate every possible arrangement of bits in a byte.

*MrrCrypt working in debug mode:*

![MrrCrypt's Adaptve Mirror Field](http://i.imgur.com/Oq2Ch7S.gif)
