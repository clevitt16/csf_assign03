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
    
    uint32_t numSets = 0, numBlocks = 0, blockSize = 0, writeAllocate = 0, writeBack = 0, lru = 0;
    // set command-line arguments

    if (atol(argv[1]) > 0) {            
	sscanf(argv[1], " %u", &numSets); 
    } else {
        fprintf(stderr, "Invalid number of sets\n");   // also need to check that sets is a power of 2
        return 1; 
    }

    if (atol(argv[2]) > 0) {
	sscanf(argv[2], " %u", &numBlocks);
    } else {
        fprintf(stderr, "Invalid number of blocks\n");  // also need to check that blocks is a power of 2
        return 1;
    }
    
    if (atol(argv[3]) >= 4) {
	sscanf(argv[3], " %u", &blockSize); 
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
        writeBack = 1U;
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

    //uint32_t wordsPerBlock = blockSize / 4;   // number of 4-byte words in each block
	
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
            Block b = {0, 0, 0, 0};
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
    int offset;
  

    // need to update all cache accesses and stores in cache to track fifo/lru
    while(scanf(" %c %x %d", &command, &address, &offset) == 3) {
        if (command == 's') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {    // cache miss
                if (writeAllocate) {  // load value into cache and change it there, implies writeBack
                    cycles += loadToCache(address, cache, lru, writeBack);  
                    cycles++; //writing to value in cache 
		    uint32_t index = computeIndex(address, cache);
		    uint32_t findAddress = searchCache(address, cache);
                    cache.sets[index].blocks[findAddress].dirty = 1; 
		    if (!writeBack) { cycles += 100; }
                } else {   // no-write-allocate - write value straight to main memory (skip over cache)
                    cycles += 100;
                }
                storeMisses++;
            } else {  // cache hit, findAddress contains block number
                if (writeBack) {  // change value in cache, make sure to mark dirty bit
                    uint32_t index = computeIndex(address, cache);
                    cache.sets[index].blocks[findAddress].dirty = 1;    // LRU
                    cycles++;
                } else {   // write-through - change value in cache and in main memory
                    cycles += 100;  // update value in main memory
                }
                storeHits++;
	//	incrementLRU(&(cache.sets[index]), findAddress);
		if (lru) {
		    uint32_t index = computeIndex(address, cache);
		    uint32_t findAddress = searchCache(address, cache);
		    incrementLRU(&(cache.sets[index]), findAddress);
		}
            }
            stores++;
	    // UPDATE NUMBER OF CYCLES
        } else if (command == 'l') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {   // cache miss
                // need to load value into cache
                loadMisses++;
		cycles += loadToCache(address, cache, lru, writeBack); // LRU AND FIFO
		cycles++; 
            } else {
                // cache hit, don't need to do anything! :)
                cycles++;
                loadHits++;
	//	incrementLRU(&(cache.sets[index]), findAddress);
		if (lru) {
                    uint32_t index = computeIndex(address, cache);
                    uint32_t findAddress = searchCache(address, cache);
                    incrementLRU(&(cache.sets[index]), findAddress);
                } 
            }
            loads++;
        } else {
            fprintf(stderr, "Invalid trace file\n");
            return 1;
        }
/*	
	if (lru && !(command == 's' && !writeAllocate)) {
	    uint32_t index = computeIndex(address, cache);
	    uint32_t findAddress = searchCache(address, cache);
	    incrementLRU(&(cache.sets[index]), findAddress);
	}

*/ 
    }

    printf("Total loads: %u\n", loads); 
    printf("Total stores: %u\n", stores);
    printf("Load hits: %u\n", loadHits);
    printf("Load misses: %u\n", loadMisses);
    printf("Store hits: %u\n", storeHits);
    printf("Store misses: %u\n", storeMisses);
    printf("Total cycles: %u\n", cycles);
    // free cache
    for (uint32_t i = 0; i < numSets; i++) {
        free(cache.sets[i].blocks);
    }
    free(cache.sets);

    return 0;
}
