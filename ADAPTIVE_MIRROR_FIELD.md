Adaptive Mirror Field
=====================

To explain what an adaptive mirror field is, let's
first expain what a typical, non-adaptive mirror field is.

Non-Adaptive Mirror Field
-------------------------

A **mirror field** is a square grid that contains randomly placed mirrors
in a diagonal orientation. Characters that can be encrypted are placed
around the perimeter of the mirror field. Encryption occurres by standing
on the character you wish to encrypt, looking straight in to the field,
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

In practice, using a typical mirror field is a weak for of encryption. The
same character of output is always produced for the same character of
input, creating a pattern of output characters similar to those used for input,
as well as hints to mirror placements and orientations of the mirror field.

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

In an *adaptive* mirror field the mirror orientation and perimeter character positions are dynamic.
For every character that is processed, the mirror field mutates itself in a way that ensures that the same path through the mirror field produces different results.
The feature creates a unique mirror field permutation for every character, eliminating the weaknesses of a traditional mirror field.

![Adaptve Mirror Field](http://i.imgur.com/bKHZ9bl.gif)

MrrCrypt's Adaptive Mirror Field
--------------------------------
