/*
 * main function for cache simulator
 * CSF Assignment 3 MS 3
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
    stringToNum(argv[1], &numSets);
    stringToNum(argv[2], &numBlocks);
    stringToNum(argv[3], &blockSize);

    setCacheConditions(argv[4], "no-write-allocate", "write-allocate", &writeAllocate); 
    setCacheConditions(argv[5], "write-through", "write-back", &writeBack); 
    setCacheConditions(argv[6], "fifo", "lru", &lru); 
    
    // error checking
    if (blockSize < 4) {
        fprintf(stderr, "Invalid cache feature\n");
        return 1;
    }
    if (!writeAllocate && writeBack) {
        fprintf(stderr, "Incompatible cache write policies\n");
        return 1;
    }

	
    // create cache - need to initialize all data
    // at this point, assume sets, blocks, and blockSize are all positive and powers of 2 (all input checking done)
    Cache cache;
    makeCache(&cache, numSets, numBlocks, blockSize); 

    // accumulators
    uint32_t loads = 0, stores = 0, loadHits = 0, loadMisses = 0, storeHits = 0, storeMisses = 0, cycles = 0;

    // reading in series of loads and stores
    char command; 
    uint32_t address;
    int offset;
  

    // need to update all cache accesses and stores in cache to track fifo/lru
    while(scanf(" %c %x %d", &command, &address, &offset) == 3) {
	uint32_t index = computeIndex(address, cache);
        uint32_t findAddress = searchCache(address, cache);
        if (command == 's') {
            if (findAddress == cache.associativity) {    // cache miss
                if (writeAllocate) {  // load value into cache and change it there, implies writeBack
                    cycles += loadToCache(address, cache, lru, writeBack);  //loading value from main, loadToCache sets lru and fifo counters
                    cycles++; //writing to value in cache 
		    uint32_t findAddress = searchCache(address, cache); //get address of block added
		    if (writeBack) {
			cache.sets[index].blocks[findAddress].dirty = 1; 
		    } else { 
			cycles += 100; //write to main because store miss and write allocate 
		    }
                } else {   // no-write-allocate - write value straight to main memory (skip over cache)
                    cycles += 100;
                }
                storeMisses++;
            } else {  // cache hit, findAddress contains block number
                if (writeBack) {  // change value in cache, make sure to mark dirty bit
                    cache.sets[index].blocks[findAddress].dirty = 1;    
                    cycles++;
                } else {   // write-through - change value in cache and in main memory
                    cycles += 100;  // update value in main memory
                }
                storeHits++;
		if (lru) {
		    incrementLRU(&(cache.sets[index]), findAddress); //hit is an access so update lru counters
		}
            }
            stores++;
        } else if (command == 'l') {
            uint32_t findAddress = searchCache(address, cache);
            if (findAddress == cache.associativity) {   // cache miss
                // need to load value into cache
                loadMisses++;
		cycles += loadToCache(address, cache, lru, writeBack); //loadToCache updates fifo and lru counters
		cycles++; 
            } else {
                // cache hit, don't need to do anything! :)
                cycles++;
                loadHits++;
		if (lru) {
                    incrementLRU(&(cache.sets[index]), findAddress); //hit is an access so update lru counters
                } 
            }
            loads++;
        } else {
            fprintf(stderr, "Invalid trace file\n");
            return 1;
        }
    }

    printf("Total loads: %u\n", loads); 
    printf("Total stores: %u\n", stores);
    printf("Load hits: %u\n", loadHits);
    printf("Load misses: %u\n", loadMisses);
    printf("Store hits: %u\n", storeHits);
    printf("Store misses: %u\n", storeMisses);
    printf("Total cycles: %u\n", cycles);
/*
    printf("Hit rate: %f\n", (1.0 * (loadHits + storeHits))/(1.0 *(loads + stores)));
    printf("Miss penalty: %u\n", (blockSize/4)*100); 
  */
    // free cache
    for (uint32_t i = 0; i < numSets; i++) {
        free(cache.sets[i].blocks);
    }
    free(cache.sets);

    return 0;
}
