# csf_assign03

MS1

Trisha and Casey both made main.c, Makefile, csimFuncs.c, and csimFuncs.h
In main.c, we are reading and validating the command line inptus, and we are reading the file stream. csimFuncs.c has the beginnings of a helper function to verify that values are powers of 2.  


MS2

Casey wrote powerOfTwo(), computeIndex(), searchCache(), and the bulk of main.c
Trisha implemented the FIFO and LRU and the loads of blocks into cache. Trisha wrote the functions loadToCache(), incrementFIFO(), incrementLRU(), findMaxCounter(), and some of main.c


Testing
Keep cache size constant - same as example 256 * 4 * 16 = 16348 bytes
Direct mapped - 1024 sets, 1 block/set, 16 bytes/block
8-way associative - 128 sets, 8 blocks/set, 16 bytes/block
4-way associative - 256 sets, 4 blocks/set, 16 bytes/block
16-way associative - 64 sets, 16 blocks/set, 16 bytes/block
Fully associative - 1 set, 1024 blocks/set, 16 bytes/block

Will test all combinatins on each cache: write-allocate/no-write-allocate, writeBack/writeThrough, lru/fifo
There are 6 combinations per cache
Will simulate all 6 combinations on both gcc.trace and swim.trace because those are from real programs

For each cache, best-performing combination will be determined by the hit rate and number of cycles
Then will determine which cache is best

RESULTS

Direct-mapped
    Best configuraiton: write-allocate write-back  (eviciton policy doesn't matter for direct mapped)
        gcc.trace:
            Hit rate: 0.969084
            Total cycles: 11127283
        swim.trace:
            Hit rate: 0.954089
            Total cycles: 10171593

8-way associative
    Best configuraiton: write-allocate write-back fifo
        gcc.trace:
            Hit rate: 0.974124
            Total cycles: 9781683
        swim.trace:
            Hit rate: 0.956876
            Total cycles: 9679193

4-way associative
    Best configuraiton: write-allocate write-back fifo
        gcc.trace:
            Hit rate: 0.973889
            Total cycles: 9845283
        swim.trace:
            Hit rate: 0.956961
            Total cycles: 9655593

16-way associative
    Best configuraiton: write-allocate write-back fifo
        gcc.trace:
            Hit rate: 0.974160
            Total cycles: 9778483
        swim.trace:
            Hit rate: 0.957021
            Total cycles: 9657593

Fully associative
    Best configuraiton: write-allocate write-back fifo
        gcc.trace:
            Hit rate: 0.974296
            Total cycles: 9734883
        swim.trace:
            Hit rate: 0.957060
            Total cycles: 9650793

Clearly, the best-performing combination of cache policies is write-allocate, write-back, and fifo.
The cache with the highest hit rates and the lowest number of cycles was the fully associative cache.
