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

int main(int argc, char **argv) {
  printf("%d\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("%s", argv[i]);
  }
  return 0;
}
