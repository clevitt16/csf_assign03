/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tctest.h"
#include "csimFuncs.h"

typedef struct {
    Cache cacheShell;
    Cache tinyCache;
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testPowerOfTwo(TestObjs *objs);
void testComputeIndex(TestObjs *objs);
void testSearchCache(TestObjs *objs);
void testLoadToCache(TestObjs *objs);

int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testPowerOfTwo);
    TEST(testComputeIndex);
	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
    Cache shell = {2, 2, NULL, 4}; // no acutal data
    objs->cacheShell = shell;
    Cache tiny; // tiny cache - one set of one block with 4 bytes
    tiny.offsetBits = 2;
    tiny.indexBits = 0;
    tiny.associativity = 1;
    Block * tinyBlockPtr = malloc(sizeof(Block));
    Block tinyBlock = {0U, 1U, 0U};
    *tinyBlockPtr = tinyBlock; // tag is 0
    Set * tinySetPtr = malloc(sizeof(Set));
    Set tinySet = {0, 1, tinyBlockPtr, 0};
	return objs;
}

void cleanup(TestObjs *objs) {

	free(objs);
}

void testPowerOfTwo(TestObjs *objs) {
	ASSERT(1 == powerOfTwo(2U));
	ASSERT(2 == powerOfTwo(4U));
    ASSERT(3 == powerOfTwo(8U));
    ASSERT(6 == powerOfTwo(64U));
    ASSERT(10 == powerOfTwo(1024U));
    ASSERT(-1 == powerOfTwo(0U));
    ASSERT(0 == powerOfTwo(1U));
    ASSERT(-1 == powerOfTwo(12U));
    ASSERT(-1 == powerOfTwo(24U));
}

void testComputeIndex(TestObjs *objs) {
    ASSERT(0U == computeIndex(0U, objs->cacheShell));
    ASSERT(0U == computeIndex(1U, objs->cacheShell));
    ASSERT(1U == computeIndex(4U, objs->cacheShell));
    ASSERT(2U == computeIndex(11U, objs->cacheShell));
    ASSERT(3U == computeIndex(255U, objs->cacheShell));
}

void testSearchCache(TestObjs *objs) {
    ASSERT(0U == searchCache(0U, objs->tinyCache));
}