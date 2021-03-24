# csf_assign03

MS1

Trisha and Casey both made main.c, Makefile, csimFuncs.c, and csimFuncs.h
In main.c, we are reading and validating the command line inptus, and we are reading the file stream. csimFuncs.c has the beginnings of a helper function to verify that values are powers of 2.  


MS2

Casey wrote powerOfTwo(), computeIndex(), searchCache(), and the bulk of main.c
Trisha implemented the FIFO and LRU and the loads of blocks into cache. Trisha wrote the functions loadToCache(), incrementFIFO(), incrementLRU(), findMaxCounter(), and some of main.c


Testing

To identify the best-performing cache configuration, we will compare 5 differently configured caches of a set size.
We keep the cache size constant in order to compare the other parameters. Furthermore, the results of this comparison
will scale to caches of any size.
The caches we tested are listed below:

Set cache size: 16348 bytes
Direct mapped - 1024 sets, 1 block/set, 16 bytes/block
8-way associative - 128 sets, 8 blocks/set, 16 bytes/block
4-way associative - 256 sets, 4 blocks/set, 16 bytes/block
16-way associative - 64 sets, 16 blocks/set, 16 bytes/block
Fully associative - 1 set, 1024 blocks/set, 16 bytes/block

For each cache, we tested all combinations of policies: write-allocate/no-write-allocate, writeBack/writeThrough, lru/fifo
There are 6 combinations per cache
For each cache, we simulated all 6 combinations on both gcc.trace and swim.trace because those are from real programs.

For each cache, the best-performing combination will be determined by the hit rate and number of cycles.
Then, we can compare the best performance from each cache to identify the best cache configuration.


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


Conclusion
    Based on our benchmarking, for a set cache size of 16348 bytes, the best-performing cache is a fully
associative cache that uses write-allocate, write-back, and fifo. However, there are several notes 
that should be made about this result. We recognize that for a fully associative cache, the larger the cache is, 
the less efficient it will be, since to find an address in the cache, the entire cache must be searched (the tag of every
block must be checked). Our simulator does not track the time required to search the cache for a given address, so this
inefficiency of fully acciative caches is not reflected in our cycle counts. Realistically, a fully associative
cache is probably not the most efficient - a partially associative cache that used index bits to narrow down the blocks 
that need to be searched would likely be faster. However, we can still confidently say that the hit rate and time spent
loading and storing values in main memory was lowest for the fully associative cache.
    Another consideration is that the way the program accesses memory will have an impact on which cache policies are
most efficient. For example, write-back and write-allocate are most effective in a program that performs sequential stores
for a large number of values, but no-write-allocate and write-through will be most effective in a program that accesses memory
from a wide range of addresses sporadically, because the evictions will be more efficient when values do not have to be written
back to main memory. The way the program uses memory will also impact whether FIFO or LRU is the most efficient eviction policy. 
For a program with sequential access patterns, FIFO will be more efficient, but for a program with a more random access pattern but
good temporal locality, LRU will be more effective.
    To increase confidence in our results for the best cache configuration, we would want to run our simulation using trace files
from a variety of different programs - since we don't know anything about the functionality of the programs associated with gcc.trace and
swim.trace, it's possible that these programs used memory in similar ways.
