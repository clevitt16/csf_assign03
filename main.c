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
        numSets = atoi(argv[1]);       // is it meant to check for overflow converting to int? or for negative numbers?
    } else {
        fprintf(stderr, "Invalid number of sets\n");   // also need to check that sets is a power of 2
        return 1; 
    }

    if (atol(argv[2]) > 0) {
        numBlocks = atoi(argv[2]);
    } else {
        fprintf(stderr, "Invalid number of blocks\n");  // also need to check that blocks is a power of 2
        return 1;
    }
    
    if (atol(argv[3]) >= 4) {
        blockSize = atoi(argv[3]);
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
    Set * sets = malloc(sizeof(Set) * numSets);
    for (uint32_t i = 0; i < numSets; i++) {
        Set set;
        Block * blocks = malloc(sizeof(Block) * numBlocks);
        for (uint32_t j = 0; j < numBlocks; j++) {
            Block b = {0, 0, 0};
            blocks[j] = b;
        }
        set.blocks = blocks;
    }

    // accumulators
    uint32_t loads = 0, stores = 0, loadHits = 0, loadMisses = 0, storeHits = 0, storeMisses = 0, cycles = 0;

    // reading in series of loads and stores
    char command; 
    uint32_t address;
    int ignore;
  
    while(scanf(" %c %x %d", &command, &address, &ignore) == 3) {
        // check for 32-bit address?

        if (command = 's') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {
                // cache miss
            } else {
                // cache hit, findAddress contains block number
            }
            stores++;
            // do cache simulation for loads, update accumulators
        } else if (command == 'l') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {
                // cache miss
            } else {
                // cache hit, findAddress contains block number
            }
            loads++;
            // so cache simultion for stores, update accumulators
        }
        else {
            fprintf(stderr, "Invalid trace file\n");
            return 1;
        }
        

        printf(" %c %d %d\n", command, address, ignore);
    }
  

    return 0;
}
