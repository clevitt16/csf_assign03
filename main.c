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
  if (argc != 7) { // 7 total - executable and 6 args
  	fprintf(stderr, "Invalid number of command line inputs\n"); 
	return 1; 
  }
  
  uint32_t sets, blocks, blockSize, writeAllocate, writeBack, lru;
  // set command-line arguments
  if (atol(argv[1]) > 0) {
	  sets = atoi(argv[1]);
  } else {
	  fprintf(stderr, "Invalid number of sets\n");
	  return 1; 
  }

  if (atol(argv[2]) > 0) {
          blocks = atoi(argv[2]);
  } else {
          fprintf(stderr, "Invalid number of blocks\n");
          return 1;
  }

  if (atol(argv[3]) >= 4) {
          blockSize = atoi(argv[3]);
  } else {
          fprintf(stderr, "Invalid block size\n");
          return 1;
  }

  if (strcmp("no-write-allocate", argv[4]) == 0) {
	  writeAllocate = 0; 
  } else if (strcmp("write-allocate", argv[4]) == 0) {
	  writeAllocate = 1; 
  } else {
	  fprintf(stderr, "Incorrect cache feature\n"); 
  }

  if (strcmp("write-through", argv[5]) == 0) {
          writeBack = 0;
  } else if (strcmp("write-back", argv[5]) == 0) {
          writeAllocate = 1;
  } else {
          fprintf(stderr, "Incorrect cache feature\n");
  }

  if (strcmp("fifo", argv[6]) == 0) {
          lru = 0;
  } else if (strcmp("lru", argv[6]) == 0) {
          lru = 1;
  } else {
          fprintf(stderr, "Incorrect cache feature\n");
  }
 
  // error checking
  if (!writeAllocate && writeBack) {
    fprintf(stderr, "Incompatible cache features\n");
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
      fprintf(stderr, "Invalid trace file\n");
      return 1;
    }
    // check for 32-bit address?
    printf(" %c %d %d\n", command, address, ignore);
  }
  

  return 0;
}
