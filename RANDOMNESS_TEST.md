Randomness Test
===============

A continuous stream of null characters were fed through `mrrcrypt` and a
number of randomness tests were applied to the output. See below for details.

Visual Analysis
---------------

Three bitmap images of varying size were generated from each of
four random keys, for a total of 12 visual analysis tests. In all cases,
the output did not show any striations or noticible patterns of any kind.

No particular bitmap seemed to favor a color, or show any bias towards
light or dark, when compared to bitmaps generated from other keys,
suggesting that different keys produce the similar random results.

Key  | 320x200 | 640x400 | 1280x800
-----|---------|---------|---------
key1 | [![TEST01](https://www.brianbarto.info/static/mrrcrypt/test01.png)](https://www.brianbarto.info/static/mrrcrypt/test01.bmp) | [![TEST05](https://www.brianbarto.info/static/mrrcrypt/test05.png)](https://www.brianbarto.info/static/mrrcrypt/test05.bmp) | [![TEST09](https://www.brianbarto.info/static/mrrcrypt/test09.png)](https://www.brianbarto.info/static/mrrcrypt/test09.bmp)
key2 | [![TEST02](https://www.brianbarto.info/static/mrrcrypt/test02.png)](https://www.brianbarto.info/static/mrrcrypt/test02.bmp) | [![TEST06](https://www.brianbarto.info/static/mrrcrypt/test06.png)](https://www.brianbarto.info/static/mrrcrypt/test06.bmp) | [![TEST10](https://www.brianbarto.info/static/mrrcrypt/test10.png)](https://www.brianbarto.info/static/mrrcrypt/test10.bmp)
key3 | [![TEST03](https://www.brianbarto.info/static/mrrcrypt/test03.png)](https://www.brianbarto.info/static/mrrcrypt/test03.bmp) | [![TEST07](https://www.brianbarto.info/static/mrrcrypt/test07.png)](https://www.brianbarto.info/static/mrrcrypt/test07.bmp) | [![TEST11](https://www.brianbarto.info/static/mrrcrypt/test11.png)](https://www.brianbarto.info/static/mrrcrypt/test11.bmp)
key4 | [![TEST04](https://www.brianbarto.info/static/mrrcrypt/test04.png)](https://www.brianbarto.info/static/mrrcrypt/test04.bmp) | [![TEST08](https://www.brianbarto.info/static/mrrcrypt/test08.png)](https://www.brianbarto.info/static/mrrcrypt/test08.bmp) | [![TEST12](https://www.brianbarto.info/static/mrrcrypt/test12.png)](https://www.brianbarto.info/static/mrrcrypt/test12.bmp)

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
   diehard_birthdays|   0|       100|     100|0.19064096|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.85214479|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.55786367|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.37725329|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.00000003|  FAILED  
        diehard_opso|   0|   2097152|     100|0.30507482|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.67000589|  PASSED  
         diehard_dna|   0|   2097152|     100|0.00193614|   WEAK   
diehard_count_1s_str|   0|    256000|     100|0.44099537|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.21341390|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.83939978|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.22561356|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.79485433|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.10100957|  PASSED  
        diehard_sums|   0|       100|     100|0.17185281|  PASSED  
        diehard_runs|   0|    100000|     100|0.08496007|  PASSED  
        diehard_runs|   0|    100000|     100|0.26859730|  PASSED  
       diehard_craps|   0|    200000|     100|0.75814612|  PASSED  
       diehard_craps|   0|    200000|     100|0.99967291|   WEAK   
 marsaglia_tsang_gcd|   0|  10000000|     100|0.90889987|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.19432853|  PASSED  
         sts_monobit|   1|    100000|     100|0.87434205|  PASSED  
            sts_runs|   2|    100000|     100|0.50274700|  PASSED  
          sts_serial|   1|    100000|     100|0.76862155|  PASSED  
          sts_serial|   2|    100000|     100|0.30717180|  PASSED  
          sts_serial|   3|    100000|     100|0.08096170|  PASSED  
          sts_serial|   3|    100000|     100|0.06255539|  PASSED  
          sts_serial|   4|    100000|     100|0.03658895|  PASSED  
          sts_serial|   4|    100000|     100|0.31469957|  PASSED  
          sts_serial|   5|    100000|     100|0.50592515|  PASSED  
          sts_serial|   5|    100000|     100|0.78526544|  PASSED  
          sts_serial|   6|    100000|     100|0.61209490|  PASSED  
          sts_serial|   6|    100000|     100|0.70897041|  PASSED  
          sts_serial|   7|    100000|     100|0.16934742|  PASSED  
          sts_serial|   7|    100000|     100|0.10423131|  PASSED  
          sts_serial|   8|    100000|     100|0.69741047|  PASSED  
          sts_serial|   8|    100000|     100|0.98645060|  PASSED    
         rgb_bitdist|   1|    100000|     100|0.03357634|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.00475478|   WEAK   
         rgb_bitdist|   3|    100000|     100|0.72723571|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.00000059|  FAILED  
         rgb_bitdist|   5|    100000|     100|0.04826931|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.99422248|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.63031114|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.00000000|  FAILED  
         rgb_bitdist|   9|    100000|     100|0.99836296|   WEAK   
         rgb_bitdist|  10|    100000|     100|0.08005160|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.67632147|  PASSED
         rgb_bitdist|  12|    100000|     100|0.09595208|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.08299197|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.86889231|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.92977986|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.06667305|  PASSED  
    rgb_permutations|   2|    100000|     100|0.84772302|  PASSED  
    rgb_permutations|   3|    100000|     100|0.93644954|  PASSED  
    rgb_permutations|   4|    100000|     100|0.29506748|  PASSED  
    rgb_permutations|   5|    100000|     100|0.25676470|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.37674403|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.06081903|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.02535086|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.48815317|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.93180633|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.62048936|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.67446474|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.41682264|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.44034447|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.17443644|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.52540818|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.05228921|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.72888208|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.12264444|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.19529781|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.98633023|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.49256745|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.89369571|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.82177762|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.24965738|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.41729803|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.23598065|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.57904976|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.18708593|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.78383985|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.61654754|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.49640843|  PASSED  
      rgb_lagged_sum|  27|   1000000|     100|0.35548801|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.49269410|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.25286305|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.84520999|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.40718616|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.11211802|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.26085400|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.40033819|  PASSED  
             dab_dct| 256|     50000|       1|0.20988867|  PASSED  
        dab_filltree|  32|  15000000|       1|0.88315290|  PASSED  
        dab_filltree|  32|  15000000|       1|0.34448474|  PASSED  
       dab_filltree2|   0|   5000000|       1|0.65433134|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.10337781|  PASSED  
        dab_monobit2|  12|  65000000|       1|1.00000000|  FAILED
```
