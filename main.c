/*
 * main function for cache simulator
 * CSF Assignment 3 MS 1
 * Casey Levitt
 * clevitt1@jh.edu
 * Trisha Karani
 * tkarani1@jh.edu
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "csimFuncs.h"

// can we establish 4-space indenting :)

int main(int argc, char **argv) {
    if (argc != 7) { // 7 total - executable and 6 args
        fprintf(stderr, "Invalid number of command line inputs\n"); 
        return 1; 
    }
    
    uint32_t numSets, numBlocks, blockSize, writeAllocate, writeBack, lru;
    // set command-line arguments

    if (atol(argv[1]) > 0) {            // kinda confused about this logic?
        numSets = atol(argv[1]);       // is it meant to check for overflow converting to int? or for negative numbers?
    } else {
        fprintf(stderr, "Invalid number of sets\n");   // also need to check that sets is a power of 2
        return 1; 
    }

    if (atol(argv[2]) > 0) {
        numBlocks = atol(argv[2]);
    } else {
        fprintf(stderr, "Invalid number of blocks\n");  // also need to check that blocks is a power of 2
        return 1;
    }
    
    if (atol(argv[3]) >= 4) {
        blockSize = atol(argv[3]);
    } else {
        fprintf(stderr, "Invalid block size\n");  // also need to check that blockSize >= 4 and a power of 2
        return 1;
    }

    if (strcmp("no-write-allocate", argv[4]) == 0) {
	    writeAllocate = 0U; 
    } else if (strcmp("write-allocate", argv[4]) == 0) {
        writeAllocate = 1U; 
    } else {
	    fprintf(stderr, "Incorrect write miss arg\n"); 
    }

    if (strcmp("write-through", argv[5]) == 0) {
        writeBack = 0U;
    } else if (strcmp("write-back", argv[5]) == 0) {
        writeAllocate = 1U;
    } else {
        fprintf(stderr, "Incorrect write hit arg\n");
    }

    if (strcmp("fifo", argv[6]) == 0) {
        lru = 0U;
    } else if (strcmp("lru", argv[6]) == 0) {
        lru = 1U;
    } else {
        fprintf(stderr, "Incorrect eviction policy\n");
    }
 
    // error checking
    if (!writeAllocate && writeBack) {
        fprintf(stderr, "Incompatible cache write policies\n");
        return 1;
    }
	
    // create cache - need to initialize all data
    // at this point, assume sets, blocks, and blockSize are all positive and powers of 2 (all input checking done)
    Cache cache;
    cache.offsetBits = powerOfTwo(blockSize);
    cache.indexBits = powerOfTwo(numSets);
    cache.associativity = numBlocks;
    Set * sets = malloc(sizeof(Set) * numSets);
    for (uint32_t i = 0; i < numSets; i++) {
        Set set;
        Block * blocks = malloc(sizeof(Block) * numBlocks);
        for (uint32_t j = 0; j < numBlocks; j++) {
            Block b = {0, 0, 0};
            blocks[j] = b;
        }
        set.blocks = blocks;
        set.numBlocks = numBlocks;
        set.emptyBlocks = numBlocks;
        sets[i] = set;
    }
    cache.sets = sets;

    // accumulators
    uint32_t loads = 0, stores = 0, loadHits = 0, loadMisses = 0, storeHits = 0, storeMisses = 0, cycles = 0;

    // reading in series of loads and stores
    char command; 
    uint32_t address;
    int ignore;
  

    // need to update all cache accesses and stores in cache to track fifo/lru
    while(scanf(" %c %x %d", &command, &address, &ignore) == 3) {
        if (command == 's') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {    // cache miss
                printf("write miss");
                if (writeAllocate) {  // load value into cache and change it there, implies writeBack
                    cycles += loadToCache(address, cache);   // LRU AND FIFO
                    cycles++; 
                } else {   // no-write-allocate - write value straight to main memory (skip over cache)
                    cycles += 100;
                }
                storeMisses++;
            } else {  // cache hit, findAddress contains block number
                printf("write hit");
                if (writeBack) {  // change value in cache, make sure to mark dirty bit
                    uint32_t index = computeIndex(address, cache);
                    cache.sets[index].blocks[findAddress].dirty = 1;    // LRU
                    cycles++;
                } else {   // write-through - change value in cache and in main memory
                    cycles++;   // update value in cache  LRU
                    cycles += 100;  // update value in main memory
                }
                storeHits++;
            }
            stores++;
	    // UPDATE NUMBER OF CYCLES
        } else if (command == 'l') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {   // cache miss
                // need to load value into cache
                printf("load miss");
                loadMisses++;
		        cycles += loadToCache(address, cache); // LRU AND FIFO
            } else {
                // cache hit, don't need to do anything! :)
                printf("load hit");
                cycles++; //Trisha! LRU
                loadHits++;
            }
            loads++;
        }
        else {
            fprintf(stderr, "Invalid trace file\n");
            return 1;
        }
        printf(" %c %d %d\n", command, address, ignore);
    }

    // free cache
    for (uint32_t i = 0; i < numSets; i++) {
        free(cache.sets[i].blocks);
    }
    free(cache.sets);

    return 0;
}
