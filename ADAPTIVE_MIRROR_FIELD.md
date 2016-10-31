Adaptive Mirror Field
=====================

To explain what an adaptive mirror field is, let's
first expain what a plain, non-adaptive mirror field is.

Non-Adaptive Mirror Field
-------------------------

A **mirror field** is a square grid that contains randomly placed diagonal
mirrors. Characters that can be encrypted are placed around the perimeter
of the mirror field. Encryption occurres by standing on the character you
wish to encrypt and looking straight in to the mirror field. The mirrors
will reflect back the encrypted character.

Mirror Field Example:

```
                A B C D E
               -----------
              K: :/: :\: :F
               -----------
              L: : :\: :/:G
  Input        -----------       Output
 ------- ==>  M:\: : :/: :H  ==> -------
 "ABCDE"       -----------       "MKEFC"
              N:\:/: : : :I
               -----------
              O: : :/: :\:J
               -----------
                P Q R S T
```

In prqactice, using this sort of mirror field for encryption is weak. The
same character of output is always produced for the same character of
input, making it easily defeatable.

```
                A B C D E
               -----------
              K: :/: :\: :F
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

In an *adaptive* mirror field the orientation of the mirrors that are used to process a character change once the processing of that character is complete.
This produces a unique mirror field permutation for every character and ensures that duplicate characters of input produce varying characters of output.

![Adaptve Mirror Field](http://i.imgur.com/sp8g4vr.gif)

While this "adaptive" feature is a big improvement, there are still weaknesses in cases where there few mirrors between the input and output characters.
In a case where there is only one mirror, given that there are only 3 mirror orientations, every third out character would repeat when given all identical input characters.

To eliminate this issue, mrrcrypt employs *perimiter character rolling*, whereby each character on the perimeter rolls it's position clockwise by 1 after every character is processed.

[Animation here]


