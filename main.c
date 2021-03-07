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

int main(int argc, char **argv) {
  assert(argc == 7); // 7 total - executable and 6 args
  // can set these using argv and strtoi
  uint32_t sets, blocks, blockSize, writeAllocate, writeThrough, lru;
  // can read strings from the trace file using scanf!
  char str[20];
  scanf("%s", str);     // can definitely put these in a loop
  printf("%s\n", str);   // i was experimenting
  scanf("%s", str);
  printf("%s\n", str);
  scanf("%s", str);
  printf("%s\n", str);
  scanf("%s", str);
  printf("%s\n", str);
  return 0;
}
