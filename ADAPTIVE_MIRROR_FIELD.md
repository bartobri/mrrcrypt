Adaptive Mirror Field
=====================

To explain what an adaptive mirror field is, let's
first expain what a plain, non-adaptive mirror field is.

Non-Adaptive Mirror Field
-------------------------

A **mirror Field** is a square grid that contains randomly placed diagonal
mirrors. Characters that can be encrypted are placed around the perimeter
of the mirror field. Encryption occurres by standing on the character you
wish to encrypt and looking straight in to the mirror field. The mirrors
will reflect back the encrypted character.

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

In prqactice, using this sort of mirror field for encryption is weak. The
same character of output is always produced for the same character of
input, making it easily defeatable.

Adaptive Mirror Field
---------------------
