#ifndef PUBLIC_H
#define PUBLIC_H

#include<glib.h>
#include<glib/gstdio.h>
#include<stdio.h>
#include<stdlib.h>
#include "subjectSetting.h"


// user defines
#define POPULATION_SIZE 50
#define MUTATION_RATE 0.01
#define CROSSOVER_RATE 0.9
#define GENERATION_MAX 100
// testcases dir, put in front of each test case and send it to the subject program in the cmd line
#ifndef DEFAULT_TESTCASES_DIR
#define DEFAULT_TESTCASES_DIR "testcases/"
#endif
#ifndef DEFAULT_CURR_DIR
#define DEFAULT_CURR_DIR "curr/"
#endif
// additional compile command for malloc.c
#ifndef DEFAULT_COMPILE_CMD
#define DEFAULT_COMPILE_CMD ""
#endif
// nanosleep() unit=10us
#define SLEEP_UNIT 10
// default timeout in sec
#ifdef TIMEOUT
#define DEFAULT_TIMEOUT_SEC TIMEOUT
#else
#define DEFAULT_TIMEOUT_SEC 2
#endif
// timeout in multiple of original time consumption
#define TIMEOUT_MULTIPLE 1.2
// sample number for testcases
#define CASES_PER_PROG 20
// # of times a testcase should be run against for
#ifndef REPEAT
#define REPEAT 1
#endif
// fitness transit ratio, f(n)=ratio*f(n-1)+(1-ratio)*g(n)
#define FITNESS_TRANSIT_RATIO 0.5
// memory profiling unit, out of date
#define MEMORY_PROFILING_UNIT peak
// verify times
#ifndef VERIFY_TIMES
#define VERIFY_TIMES 10
#endif
// user defines

#define DEFAULT_GCC_MALLOC "gcc %s -shared -fPIC -o %s -O3 malloc.c %s"

#define SHALLOW_GCC_MALLOC "gcc %s -w -shared -fPIC -o %s -O3 malloc.c %s -D\'MALLOC_ALIGNMENT=((size_t)(%d*sizeof(void*)))\' -DFOOTERS=%d -DINSECURE=%d -DNO_SEGMENT_TRAVERSAL=%d -DMORECORE_CONTIGUOUS=%d -DDEFAULT_GRANULARITY=%d -DDEFAULT_TRIM_THRESHOLD=%d -DDEFAULT_MMAP_THRESHOLD=%d -DMAX_RELEASE_CHECK_RATE=%d"

gint populationSize;
gint generationMax;
gint repeatTimes;
double mutationRate;
double crossoverRate;
gint numberOfGenes;
double timeout_sec;
gint randomSearch;
gint extraEvaluation;
gint sampleSize;
double samplingGeneration;

gint profile_times;

// the times should be no less than (1+CROSSOVER_RATE)
gint COMBINED_POPULATION_SIZE;

#define NUMBER_OF_SHALLOW_GENE 6
typedef enum _gene{
	malloc_alignment,
	malloc_footer_insecure_no_segment_traversal,
	malloc_default_granularity,
	malloc_default_trim_threshold,
	malloc_default_mmap_threshold,
	malloc_max_release_check_rate,
	malloc_expose_0,
	malloc_expose_1,
	malloc_expose_2,
	malloc_expose_3,
	malloc_expose_4,
	malloc_expose_5,
	malloc_expose_6,
	malloc_expose_7,
	malloc_expose_8,
	NUMBER_OF_GENE
}gene;

#define FOOTER_BIT 1
#define INSECURE_BIT 2
#define NO_SEGMENT_TRAVERSAL_BIT 4
#define MORECORE_CONTIGUOUS_BIT 8

typedef enum _mutationType{
	mutation_one,
	mutation_gap,
	mutation_random,
	mutation_power2,
	mutation_power2_random,
	mutation_power2_allow0,
	mutation_power2_allow0_random,
	mutation_boolean
}mutType;

typedef struct _individual{
	gint chrom[NUMBER_OF_GENE];
	double time;
	double time_usr;
	double time_sys;
	double memory;
	double failNum;
	gint evaluateTimes;
	double time_repeat[REPEAT];
	gint paretoLevel;
	double crowdDistance;
	struct _individual** dominateSet;
	gint dominateCount;
	gint beDominatedCount;
	gint wasSelected;
}individual;

typedef struct _memoryUsage{
	double avg;
	double peak;
	double timeSys;
	double timeUsr;
	double failNum;
}memoryUsage;

GList* testcases;

GList* testcaseResults;

GList* stdMemoryAvg;

individual* ori;

gint fullTest;

extern const gchar TESTCASEDIR[];

extern const gchar CURRDIR[];

extern const gchar GCC_M32[];

extern const gint default_individual[];

extern const gint default_lower_bound[];

extern const gint default_upper_bound[];

extern const mutType default_mutation_type[];

FILE* logfp;

gint saveIndividual(individual*, gchar*, gint);

void printIndividual(individual*);

void savePopulation(GList* , gint);

void saveSelectivePopulation(GList*, gchar*);

individual* copyIndividual(individual*);

void free_individual(individual*);

// compare two individuals according to their front level and crowd distance
// return 1 if the first one is better, -1 the second one is better, 0 the same
gint cmpIndividual(individual*, individual*);

// compare two files, returns 0 if identical, 1 otherwise
gint cmpOutput(gchar*, gchar*);

#endif // PUBLIC_H
