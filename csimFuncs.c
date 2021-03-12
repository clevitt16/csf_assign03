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


uint32_t powerOfTwo (uint32_t num) {
    if (num == 0) { // zero is not a positive power of two
        return 0U;
    }
    uint32_t exp = 1U;   // will keep track of the exponent
    while (num % 2 == 0) {
        if (num == 2) {     // powers of two
            return exp;
        }
        exp++;
        num /= 2;
    }
  return 0U;    // if num % 2 != 0, it's not a power of 2
}

uint32_t searchCache (uint32_t address, Cache cache) {
    // need to extract index from address - will create a bitstring 111000 where 1s are index, 0s offset
    uint32_t getIndex = (uint32_t)pow(2, cache.indexBits);
    getIndex = getIndex << cache.offsetBits;
    uint32_t index = address & getIndex;
    index = index >> cache.offsetBits;
    Set set = cache.sets[index];
    uint32_t tag = address >> (cache.indexBits + cache.offsetBits);
    for (uint32_t i = 0U; i < cache.associativity; i++) {
        if (set.blocks[i].valid && set.blocks[i].tag == tag) {
            return i;
        }
    }
    return cache.associativity;
}

