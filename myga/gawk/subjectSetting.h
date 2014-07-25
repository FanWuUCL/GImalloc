#ifndef SETTING_H
#define SETTING_H

// testcases dir, put in front of each test case and send it to the subject program in the cmd line
#define DEFAULT_TESTCASES_DIR ""
#define DEFAULT_CURR_DIR "curr/"
// additional compile command for malloc.c
#define DEFAULT_COMPILE_CMD ""

// # of times a testcase should be run against for
#define REPEAT 5

#define DEEP_GCC_MALLOC "gcc %s -w -shared -fPIC -o %s -O3 malloc.c %s -D\'MALLOC_ALIGNMENT=((size_t)(%d*sizeof(void*)))\' -DFOOTERS=%d -DINSECURE=%d -DNO_SEGMENT_TRAVERSAL=%d -DMORECORE_CONTIGUOUS=%d -DDEFAULT_GRANULARITY=%d -DDEFAULT_TRIM_THRESHOLD=%d -DDEFAULT_MMAP_THRESHOLD=%d -DMAX_RELEASE_CHECK_RATE=%d -DGAWK_4301=%d -DGAWK_4334=%d -DGAWK_4425=%d -DGAWK_4399=%d -DGAWK_4544=%d -DGAWK_4537=%d -DGAWK_4428=%d -DGAWK_4941=%d -DGAWK_4602=%d"

#define DEFAULT_IND 2, 8, 0, 2048, 256, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0

#define LOWERBOUND_IND 1, 0, 4, 64, 16, 1000, -4096, -32, -1024, -64, -1, -32, -4096, -8, -1

#define UPPERBOUND_IND 16, 15, 512, 16*1024, 2048, 10000, 4096, 256, 1024, 64, 1, 32, 4096, 8, 1

#define MUTATIONTYPE_IND mutation_power2, mutation_gap, mutation_power2_allow0, mutation_power2, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_one, mutation_gap, mutation_gap, mutation_random, mutation_one

#endif // SETTING_H
