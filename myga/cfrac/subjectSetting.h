#ifndef SETTING_H
#define SETTING_H

// testcases dir, put in front of each test case and send it to the subject program in the cmd line
#define DEFAULT_TESTCASES_DIR ""
#define DEFAULT_CURR_DIR "curr/"
// additional compile command for malloc.c
#define DEFAULT_COMPILE_CMD "-m32"

// # of times a testcase should be run against for
#define REPEAT 1

#define DEEP_GCC_MALLOC "gcc %s -w -shared -fPIC -o %s -O3 malloc.c -D\'MALLOC_ALIGNMENT=((size_t)(%d*sizeof(void*)))\' -DFOOTERS=%d -DINSECURE=%d -DNO_SEGMENT_TRAVERSAL=%d -DMORECORE_CONTIGUOUS=%d -DDEFAULT_GRANULARITY=%d -DDEFAULT_TRIM_THRESHOLD=%d -DDEFAULT_MMAP_THRESHOLD=%d -DMAX_RELEASE_CHECK_RATE=%d -DCFRAC_4345=%d -DCFRAC_4316=%d -DCFRAC_4540=%d -DCFRAC_4866=%d -DCFRAC_4830=%d -DCFRAC_4297=%d -DCFRAC_4794=%d -DCFRAC_4334=%d -DCFRAC_4313=%d"

#define DEFAULT_IND 2, 8, 0, 2048, 256, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0

#define LOWERBOUND_IND 1, 0, 4, 64, 16, 1000, -1024, -64, -1024, -32, 0, 0, -32, -4096, -1

#define UPPERBOUND_IND 16, 15, 512, 16*1024, 2048, 10000, 1024, 64, 1024, 32, 1, 1, 32, 4096, 9

#define MUTATIONTYPE_IND mutation_power2, mutation_random, mutation_power2_allow0, mutation_power2, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random, mutation_random

#endif // SETTING_H
