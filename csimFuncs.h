/*
 * main function for cache simulator
 * CSF Assignment 3 MS 1
 * Casey Levitt
 * clevitt1@jh.edu
 * Trisha Karani
 * tkarani1@jh.edu
 *
 */

#ifndef CSIMFUNCS_H
#define CSIMFUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>


typedef struct {
    uint32_t dirty;  // will only be used for write-back caches
    uint32_t * tags; // holds the tags that identify each block
} Block;

typedef struct {
    uint32_t index;     // identifies the set
    uint32_t numBlocks;
    Block * blocks;   
} Set;

typedef struct {
    uint32_t offsetBits;
    uint32_t indexBits;
    uint32_t numSets;
    Set * sets;
} Cache;


/*
* Checks if given unsigned is a power of two
* Parameters
*     num - unsigned int to check
* Returns
*     0 if num is not a power of 2
*     if num is a power of 2, returns the exponent
*
*/
uint32_t powerOfTwo (uint32_t num);

/*
* Checks if given address is a hit or a miss
*
*/
uint32_t searchCache (uint32_t address);


#endif
