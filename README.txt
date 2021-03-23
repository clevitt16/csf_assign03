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

Each tests with write-allocate, no-write-allocate, writeBack, writeThrough, lru, fifo
Six tests per associativity

Run on gcc.trace and swim.trace because those are from real programs

