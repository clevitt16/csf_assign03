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
    Cache cacheShell; // no actual data, (sets is NULL)
    Cache tinyCache; // tiny cache - one set of one 4-byte block, will start full
    Cache twosCache; // 2 sets, 2 4-byte blocks per set, will start with 1 block in each set full
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testPowerOfTwo(TestObjs *objs);
void testComputeIndex(TestObjs *objs);
void testSearchCache(TestObjs *objs);
void testLoadToCache(TestObjs *objs);

/* set to nonzero if a call to exit is expected */
int expectedExit;
/* jump buffer that our version of exit can use to jump back to test function */
sigjmp_buf exitBuf;

/*
 * Custom version of exit: useful for testing functions where
 * the expected behavior is a call to exit (e.g., because
 * an argument value is invalid.)  If the expectedExit
 * variable is set to a nonzero value, uses siglongjmp (via
 * exitBuf) to return control to the test function.
 * If expectedExit is not set, immediately fails the
 * current test.
 */
void exit(int exitCode) {
        if (expectedExit) {
                /* jump back to test function */
                siglongjmp(exitBuf, 1);
        } else {
                /* exit called unexpectedly, fail the test */
                FAIL("Unexpected exit");
        }
}

void on_complete(int numPassed, int numExecuted) {
        printf("%d/%d tests passed\n", numPassed, numExecuted);
}



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
    TEST(testSearchCache);
    TEST(testLoadToCache);
	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
    // CREATE SHELL
    Cache shell = {2, 2, NULL, 4};
    objs->cacheShell = shell;
    // CREATE TINYCACHE
    Cache tiny; 
    tiny.offsetBits = 2; // blocksize 4
    tiny.indexBits = 0; // 1 set
    tiny.associativity = 1; // 1 block per set
    Block * tinyBlockPtr = malloc(sizeof(Block));
    Block tinyBlock = {0U, 1U, 0U}; // INITIAL DATA has tag 0
    *tinyBlockPtr = tinyBlock;
    Set * tinySetPtr = malloc(sizeof(Set));
    Set tinySet;
    tinySet.blocks = tinyBlockPtr;
    tinySet.emptyBlocks = 0;
    tinySet.numBlocks = 1;
    *tinySetPtr = tinySet;
    tiny.sets = tinySetPtr;
    objs->tinyCache = tiny;
    // CREATE TWOSCACHE
    Cache twos; 
    twos.offsetBits = 2; // blocksize 4
    twos.indexBits = 1; // 2 sets
    twos.associativity = 2; // 2 blocks per set
    Set * twosSetPtr = malloc(sizeof(Set) * 2);
    for (int i = 0; i < 2; i++) {
        Set set;
        Block * blocks = malloc(sizeof(Block) * 2);
        Block block1 = {0U, 1U, 1U}; // INITIAL DATA has tag 1
        Block block2 = {0U, 0U, 0U};
        blocks[0] = block1;
        blocks[1] = block2;
        set.blocks = blocks;
        set.numBlocks = 2;
        set.emptyBlocks = 1;
        twosSetPtr[i] = set;
    }
    twos.sets = twosSetPtr;
    objs->twosCache = twos;
	return objs;
}

void cleanup(TestObjs *objs) {
    free(objs->tinyCache.sets[0].blocks);
    free((objs->tinyCache.sets));
    free(objs->twosCache.sets[0].blocks);
    free(objs->twosCache.sets[1].blocks);
    free(objs->twosCache.sets);
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
    // tests with tinyCache
    ASSERT(0U == searchCache(0U, objs->tinyCache));
    ASSERT(objs->tinyCache.associativity == searchCache(255U, objs->tinyCache));
    objs->tinyCache.sets[0].blocks[0].tag = 2U;
    ASSERT(0U == searchCache(8U, objs->tinyCache));
    ASSERT(objs->tinyCache.associativity == searchCache(255U, objs->tinyCache));
    // test with twosCache
    ASSERT(0U == searchCache(8U, objs->twosCache));
    ASSERT(0U == searchCache(12U, objs->twosCache));
    ASSERT(objs->twosCache.associativity == searchCache(255U, objs->twosCache));
}

// need to add more args to tests
void testLoadToCache(TestObjs *objs) {
    // tests with tinyCache
    loadToCache(12U, objs->tinyCache);
    ASSERT(0U == searchCache(12U, objs->tinyCache));
    ASSERT(objs->tinyCache.associativity == searchCache(0U, objs->tinyCache));
    loadToCache(16U, objs->tinyCache);
    ASSERT(0U == searchCache(16U, objs->tinyCache));
    ASSERT(objs->tinyCache.associativity == searchCache(12U, objs->tinyCache));
    // test with twosCache

}

