Randomness Test
===============

A continuous stream of null characters were fed through `mrrcrypt` and a
number of randomness tests were applied to the output. See below for details.

Visual Analysis
---------------

Three bitmap bitmap images of varying size were generated from each of
four random keys, for a total of 12 visual analysis tests. In all cases,
the output did not show any striations or noticible patterns of any kind.

No particular bitmap seemed to favor a color, or show any bias towards
light or dark, when compared to bitmaps generated from other keys,
suggesting that different keys produce the similar random results.

Key  | 320x200 | 640x400 | 1280x800
-----|---------|---------|---------
key1 | [![TEST01](http://brianbarto.info/extern/images/mrrcrypt/test01.png)](http://brianbarto.info/extern/images/mrrcrypt/test01.bmp) | [![TEST05](http://brianbarto.info/extern/images/mrrcrypt/test05.png)](http://brianbarto.info/extern/images/mrrcrypt/test05.bmp) | [![TEST09](http://brianbarto.info/extern/images/mrrcrypt/test09.png)](http://brianbarto.info/extern/images/mrrcrypt/test09.bmp)
key2 | [![TEST02](http://brianbarto.info/extern/images/mrrcrypt/test02.png)](http://brianbarto.info/extern/images/mrrcrypt/test02.bmp) | [![TEST06](http://brianbarto.info/extern/images/mrrcrypt/test06.png)](http://brianbarto.info/extern/images/mrrcrypt/test06.bmp) | [![TEST10](http://brianbarto.info/extern/images/mrrcrypt/test10.png)](http://brianbarto.info/extern/images/mrrcrypt/test10.bmp)
key3 | [![TEST03](http://brianbarto.info/extern/images/mrrcrypt/test03.png)](http://brianbarto.info/extern/images/mrrcrypt/test03.bmp) | [![TEST07](http://brianbarto.info/extern/images/mrrcrypt/test07.png)](http://brianbarto.info/extern/images/mrrcrypt/test07.bmp) | [![TEST11](http://brianbarto.info/extern/images/mrrcrypt/test11.png)](http://brianbarto.info/extern/images/mrrcrypt/test11.bmp)
key4 | [![TEST04](http://brianbarto.info/extern/images/mrrcrypt/test04.png)](http://brianbarto.info/extern/images/mrrcrypt/test04.bmp) | [![TEST08](http://brianbarto.info/extern/images/mrrcrypt/test08.png)](http://brianbarto.info/extern/images/mrrcrypt/test08.bmp) | [![TEST12](http://brianbarto.info/extern/images/mrrcrypt/test12.png)](http://brianbarto.info/extern/images/mrrcrypt/test12.bmp)

NIST and Diehard Tests
----------------------

Output was tested for randmoness using the
[Statistical Test Suite](http://csrc.nist.gov/groups/ST/toolkit/rng/stats_tests.html)
developed by the National Institute for Standards and Technology (NIST),
as well as other *reliable* tests that are incorporated into the
[diehard test suite](https://en.wikipedia.org/wiki/Diehard_tests).

Below are the results of such tests:

```
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.24363155|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.76808118|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.48295991|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.00000000|  FAILED  
   diehard_bitstream|   0|   2097152|     100|0.83054800|  PASSED  
        diehard_opso|   0|   2097152|     100|0.00000000|  FAILED  
        diehard_oqso|   0|   2097152|     100|0.92350313|  PASSED  
         diehard_dna|   0|   2097152|     100|0.40951043|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.30512110|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.64460081|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.00000000|  FAILED  
    diehard_2dsphere|   2|      8000|     100|0.76287597|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.23172155|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.00000000|  FAILED  
        diehard_sums|   0|       100|     100|0.79703966|  PASSED  
        diehard_runs|   0|    100000|     100|0.13315721|  PASSED  
        diehard_runs|   0|    100000|     100|0.24898782|  PASSED  
       diehard_craps|   0|    200000|     100|0.00000000|  FAILED  
       diehard_craps|   0|    200000|     100|0.19117045|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000000|  FAILED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000000|  FAILED  
         sts_monobit|   1|    100000|     100|0.77713306|  PASSED  
            sts_runs|   2|    100000|     100|0.00000000|  FAILED
          sts_serial|   1|    100000|     100|0.65547011|  PASSED  
          sts_serial|   2|    100000|     100|0.00000003|  FAILED  
          sts_serial|   3|    100000|     100|0.00015532|   WEAK   
          sts_serial|   3|    100000|     100|0.98215786|  PASSED  
          sts_serial|   4|    100000|     100|0.00127527|   WEAK   
          sts_serial|   4|    100000|     100|0.89186692|  PASSED  
          sts_serial|   5|    100000|     100|0.08041059|  PASSED  
          sts_serial|   5|    100000|     100|0.82334954|  PASSED  
          sts_serial|   6|    100000|     100|0.03763642|  PASSED  
          sts_serial|   6|    100000|     100|0.28774795|  PASSED  
          sts_serial|   7|    100000|     100|0.00778034|  PASSED  
          sts_serial|   7|    100000|     100|0.15379860|  PASSED  
          sts_serial|   8|    100000|     100|0.00405193|   WEAK   
          sts_serial|   8|    100000|     100|0.04428714|  PASSED  
          sts_serial|   9|    100000|     100|0.07420246|  PASSED  
          sts_serial|   9|    100000|     100|0.20229072|  PASSED  
          sts_serial|  10|    100000|     100|0.35353037|  PASSED  
          sts_serial|  10|    100000|     100|0.81962865|  PASSED  
          sts_serial|  11|    100000|     100|0.34274258|  PASSED  
          sts_serial|  11|    100000|     100|0.36249314|  PASSED  
          sts_serial|  12|    100000|     100|0.84151379|  PASSED  
          sts_serial|  12|    100000|     100|0.99801843|   WEAK   
          sts_serial|  13|    100000|     100|0.89779226|  PASSED  
          sts_serial|  13|    100000|     100|0.71070490|  PASSED  
          sts_serial|  14|    100000|     100|0.77122111|  PASSED  
          sts_serial|  14|    100000|     100|0.50111881|  PASSED  
          sts_serial|  15|    100000|     100|0.20276710|  PASSED  
          sts_serial|  15|    100000|     100|0.75367511|  PASSED  
          sts_serial|  16|    100000|     100|0.39407119|  PASSED  
          sts_serial|  16|    100000|     100|0.80251525|  PASSED
         rgb_bitdist|   1|    100000|     100|0.02542659|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.00133307|   WEAK   
         rgb_bitdist|   3|    100000|     100|0.00379700|   WEAK   
         rgb_bitdist|   4|    100000|     100|0.26457318|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.14538028|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.92288553|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.65084874|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.75341506|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.97159230|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.91094955|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.04481010|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.26587670|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.23076153|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.28653929|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.04473101|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.45574187|  PASSED  
    rgb_permutations|   2|    100000|     100|0.85020266|  PASSED  
    rgb_permutations|   3|    100000|     100|0.97427932|  PASSED  
    rgb_permutations|   4|    100000|     100|0.33292543|  PASSED  
    rgb_permutations|   5|    100000|     100|0.64533475|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.00000000|  FAILED  
     rgb_kstest_test|   0|     10000|    1000|0.00000130|   WEAK
     dab_bytedistrib|   0|  51200000|       1|0.00000000|  FAILED
             dab_dct| 256|     50000|       1|0.45241142|  PASSED
        dab_filltree|  32|  15000000|       1|0.27920628|  PASSED  
        dab_filltree|  32|  15000000|       1|0.89654691|  PASSED
       dab_filltree2|   0|   5000000|       1|0.02907675|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.00000000|  FAILED
        dab_monobit2|  12|  65000000|       1|1.00000000|  FAILED
```
