#ifndef SETTING_H
#define SETTING_H

// testcases dir, put in front of each test case and send it to the subject program in the cmd line
#define DEFAULT_TESTCASES_DIR ""
#define DEFAULT_CURR_DIR "curr/"
// additional compile command for malloc.c
#define DEFAULT_COMPILE_CMD ""

// # of times a testcase should be run against for
#define REPEAT 1

#define DEEP_GCC_MALLOC "gcc %s -w -shared -fPIC -o %s -O3 malloc.c %s -D\'MALLOC_ALIGNMENT=((size_t)(%d*sizeof(void*)))\' -DFOOTERS=%d -DINSECURE=%d -DNO_SEGMENT_TRAVERSAL=%d -DMORECORE_CONTIGUOUS=%d -DDEFAULT_GRANULARITY=%d -DDEFAULT_TRIM_THRESHOLD=%d -DDEFAULT_MMAP_THRESHOLD=%d -DMAX_RELEASE_CHECK_RATE=%d -DSPACE_4301=%d -DSPACE_4547=%d -DSPACE_4425=%d -DSPACE_4866=%d -DSPACE_4399=%d -DSPACE_4346=%d -DSPACE_4854=%d -DSPACE_4537=%d -DSPACE_4334=%d"

#define DEFAULT_IND 2, 8, 0, 2048, 256, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0

#define LOWERBOUND_IND 1, 0, 4, 64, 16, 1000, 0, -4096, -1024, -32, -64, -4096, -128, -32, -32

#define UPPERBOUND_IND 16, 15, 512, 16*1024, 2048, 10000, 1, 4096, 1024, 32, 64, 4096, 128, 32, 256

#define MUTATIONTYPE_IND mutation_power2, mutation_gap, mutation_power2_allow0, mutation_power2, mutation_gap, mutation_gap, mutation_boolean, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_gap, mutation_gap

#define CUSTOM_EXTRA_SAVING g_snprintf(filename, 128, "%slex.yy.c%d%s", CURRDIR, j, append);\
	if(g_file_test("lex.yy.c", G_FILE_TEST_EXISTS)) mycp("lex.yy.c", filename);\
	g_snprintf(filename, 128, "%serr_flex%d%s", CURRDIR, j, append);\
	mycp("err_flex", filename);

#define CUSTOM_CORRECTNESS for(i=0; i<length; i++){\
				g_snprintf(filename, 128, "%slex.yy.c%d", CURRDIR, i);\
				g_snprintf(line, 128, "%slex.yy.c%d.s", CURRDIR, i);\
				if(g_file_test(filename, G_FILE_TEST_EXISTS)){\
					if(g_file_test(line, G_FILE_TEST_EXISTS)) correctness_t+=cmpOutput(filename, line);\
					else correctness_t+=1;\
				}\
				else if(g_file_test(line, G_FILE_TEST_EXISTS)) correctness_t+=1;\
				else{\
					g_snprintf(filename, 128, "%serr_flex%d", CURRDIR, i);\
					g_snprintf(line, 128, "%serr_flex%d.s", CURRDIR, i);\
					correctness_t+=cmpOutput(filename, line);\
				}\
			}

#endif // SETTING_H
