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

int main(int argc, char **argv) {
  assert(argc == 7); // 7 total - executable and 6 args
  uint32_t sets, blocks, blockSize, writeAllocate, writeBack, lru;
  // set command-line arguments
  sets = atoi(argv[1]);
  blocks = atoi(argv[2]);
  blockSize = atoi(argv[3]);
  writeAllocate = strcmp("no-write-allocate", argv[4]); // will return zero if they're equal
  writeAllocate = writeAllocate == 0 ? 0 : 1;
  writeBack = strcmp("write-through", argv[5]); // will return zero if they're equal
  writeBack = writeBack == 0 ? 0 : 1;
  lru = strcmp("lru", argv[6]) == 0 ? 1 : 0;
  // error checking
  if (!writeAllocate && writeBack) {
    fprintf(stderr, "Incompatible cache features");
    return 1;
  }
  // reading in series of loads and stores
  char command; 
  uint32_t address;
  int ignore;
  //printf("%d", scanf(" %c %x %d ", &command, &address, &ignore));
  //printf("%c %x %d", command, address, ignore);
  
  while(scanf(" %c %x %d", &command, &address, &ignore) == 3) {
    // error checking
    if (command != 's' && command != 'l') {
      fprintf(stderr, "Invalid trace file");
      return 1;
    }
    // check for 32-bit address?
    printf(" %c %x %d\n", command, address, ignore);
  }
  

  return 0;
}
