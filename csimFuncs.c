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


uint32_t isPowerOfTwo (uint32_t num) {
  if (num == 0) { // zero is not a positive power of two
    return 0U;
  }
  while (num % 2 == 0) {
    if (num == 2) { // powers of two
      return 1U;
    }
    num /= 2;
  }
  return 0U;
}
