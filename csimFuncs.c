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
    Set set = cache.sets[index]; // 
    uint32_t tag = address >> (cache.indexBits + cache.offsetBits);
    for (uint32_t i = 0U; i < cache.associativity; i++) {
        if (set.blocks[i].valid && set.blocks[i].tag == tag) {
            return i;
        }
    }
    return cache.associativity;
}


uint32_t loadToCache (uint32_t address, Cache cache) {
/*
* compute index, locate set
* if emptyBlocks > 0
*   iterate through blocks until you find the invalid one
*   done
* if emptyBlocks == 0 need to do an eviction!
*   identify which block to evict
*       will need to add data to blocks to indicate fifo/lru tracking
*   if write-through, can just evict the block
*   if write-back, need to store entire block in main memory
*  
* set valid and tag bit of your new block  
*
*/
    return 0;
}

