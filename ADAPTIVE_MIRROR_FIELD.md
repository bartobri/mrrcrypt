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

In an *adaptive* mirror field the orientation of the mirrors are dynamic. They change orientation for each character that is processed through the mirror field.
This produces a unique mirror field permutation for every character, and ensures that duplicate characters of input produce varying characters of output, eliminating the weaknesses of a traditional mirror field.

![Adaptve Mirror Field](http://i.imgur.com/sp8g4vr.gif)

While this **adaptive** feature is a big improvement, there is still a potential weakness in a case where there are few mirrors between the input and output characters.
For example, in a case where there is only one mirror, given that there are 3 mirror orientations, every third output character would repeat when given all identical input characters.

To eliminate this issue, MrrCrypt employs *perimiter character rolling*. Each character on the perimeter rolls it's position clockwise by 1 after every character is processed.

![Adaptve Mirror Field](http://i.imgur.com/bKHZ9bl.gif)


