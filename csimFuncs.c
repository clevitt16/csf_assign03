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
#include <math.h>

// need to change, 1 is 2^0, valid
int powerOfTwo (uint32_t num) {
    if (num == 0) { // zero is not a positive power of two
        return -1;
    } else if (num == 1U) {
        return 0;
    }
    uint32_t exp = 1U;   // will keep track of the exponent
    while (num % 2 == 0) {
        if (num == 2) {     // powers of two
            return exp;
        }
        exp++;
        num /= 2;
    }
  return -1;    // if num % 2 != 0, it's not a power of 2
}

uint32_t computeIndex (uint32_t address, Cache cache) {
    // need to extract index from address - will create a bitstring 111000 where 1s are index, 0s offset
    if (cache.indexBits == 0U) {  // fully-associative cache
        return 0U;
    }
    uint32_t getIndex = (uint32_t)pow(2, cache.indexBits) - 1U; // will be at least 1
    getIndex = getIndex << cache.offsetBits;
    uint32_t index = address & getIndex;
    index = index >> cache.offsetBits;
    return index;
}


uint32_t searchCache (uint32_t address, Cache cache) {
    uint32_t index = computeIndex(address, cache);
    Set set = cache.sets[index]; 
    uint32_t tag = address >> (cache.indexBits + cache.offsetBits);
    for (uint32_t i = 0U; i < cache.associativity; i++) {
        if (set.blocks[i].valid && set.blocks[i].tag == tag) {
            return i;
        }
    }
    return cache.associativity;
}

uint32_t findMaxCounter(Set * s) {
    uint32_t max = 0, maxIdx = 0;
    max = s->blocks[0].counter; //assume all blocks are valid
    for (uint32_t i = 1; i<s->numBlocks; i++) {
            if (s->blocks[i].counter > max) {
                max = s->blocks[i].counter;
                maxIdx = i;
            }
    }
    return maxIdx;
}

void incrementLRU(Set * s, uint32_t idxToAccess) {
    uint32_t accessCounter = s->blocks[idxToAccess].counter; 
    if (accessCounter == 0) { //removing most recently used, no change
	return; 
    } 

    for (uint32_t i=0; i<s->numBlocks; i++) { //increase other counters
	if (s->blocks[i].valid && s->blocks[i].counter <= accessCounter) {
	    s->blocks[i].counter++; 
	}
    }
    
    s->blocks[idxToAccess].counter = 0; 

}

void incrementFIFO(Set * s, uint32_t idxToAccess) {
    for (uint32_t i=0; i<s->numBlocks; i++) { //increase other counters
	s->blocks[i].counter++;
    }
    
    s->blocks[idxToAccess].counter = 0;

}

uint32_t loadToCache (uint32_t address, Cache cache, uint32_t lru, uint32_t writeBack) {
    uint32_t wordsPerBlock = (unsigned)pow(2, cache.offsetBits - 2); // number of 4-byte chunks in each block
    uint32_t cycles = wordsPerBlock * 100; // each 4-byte word takes 100 cycles to load
    Block * b = NULL; 
    uint32_t index = computeIndex(address, cache);
    Set * s = &(cache.sets[index]); ;
    if (s->emptyBlocks >  0) {
	    for (uint32_t i = 0; i < s->numBlocks; i++) { //find empty block
	        if (s->blocks[i].valid == 0) {
		        b = &(s->blocks[i]); //will load data into this block
			break;
	        }
	    }
	    s->emptyBlocks--; 
    } else { //0 empty blocks in set, need to evict
	    uint32_t idxToEvict = findMaxCounter(s); 
	    if (lru) {
    		incrementLRU(s, idxToEvict);  
	    } else {
		incrementFIFO(s, idxToEvict); 
	    }
	    b = &(s->blocks[idxToEvict]);
	    if (writeBack) {
	        if (b->dirty) {
		        cycles += wordsPerBlock * 100; //have to write the block back to memory
	        }
	    } 
    } 	
    b->valid = 1; 
    b->tag = address >> (cache.indexBits + cache.offsetBits);
    b->dirty = 0;
    return cycles;
	
}

void stringToNum(char * input, uint32_t * num) {
    if (atol(input) > 0) {
        sscanf(input, " %u", num);
	if (powerOfTwo(*num) == -1) {
	    fprintf(stderr, "Cache feature not a power of two\n");
	    exit(1); 
	}
    } else {
        fprintf(stderr, "Invalid cache feature\n");  
        exit(1);
    }
}

void setCacheConditions(char * input, char * zero, char * one, uint32_t * num) {
    if (strcmp(zero, input) == 0) {
            *num = 0U;
	    return;
    } else if (strcmp(one, input) == 0) {
        *num = 1U;
	return;
    } else {
            fprintf(stderr, "Incorrect write miss arg\n");
	    exit(1); 
    }
}

void makeCache(Cache * cache, uint32_t numSets, 
		uint32_t numBlocks, uint32_t blockSize) { 
    cache->offsetBits = powerOfTwo(blockSize);
    cache->indexBits = powerOfTwo(numSets);
    cache->associativity = numBlocks;
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
    cache->sets = sets;
}

/*
Case 1: Store 
    Not in cache 
	if write_alloc
	    cycles + load_cycles //load to cache
	    cycles + 1  //write to cache
	    if write_through
		cycles + 100 //write to main mem
	else if no_write_alloc
	    cycles + 100 //write to main

    In cache
	if write_back
	    cycles + 1 //write to cache
	else if write through
	    cycles + 100 //write to main

Case 2: Load
    Not in cache
	cycles + load_cycles //load to cache


    In cache
	cycles + 1 //read from cache

*/





