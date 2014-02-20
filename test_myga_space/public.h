#ifndef PUBLIC_H
#define PUBLIC_H

#include<glib.h>
#include<glib/gstdio.h>
#include<stdio.h>
#include<stdlib.h>

#define MYGP

// user defines
#define POPULATION_SIZE 50
#define MUTATION_RATE 0.2
#define CROSSOVER_RATE 0.8
#define GENERATION_MAX 20
// the times should be no less than (1+MUTATION_RATE+CROSSOVER_RATE)
#define COMBINED_POPULATION_SIZE (POPULATION_SIZE*2)
// testcases dir, put in front of each test case and send it to the subject program in the cmd line
#define DEFAULT_TESTCASES_DIR "testcases/"
#define DEFAULT_CURR_DIR "curr/"
// additional compile command for malloc.c
#define DEFAULT_COMPILE_CMD ""
// usleep() parameter, also the precision of profiled time
#define SLEEP_UNIT 100
// sample number for testcases
#define CASES_PER_PROG 20
// # of times a testcase should be run against for
#define REPEAT 1
// fitness transit ratio, f(n)=ratio*f(n-1)+(1-ratio)*g(n)
#define FITNESS_TRANSIT_RATIO 0.5
// memory profiling unit, out of date
#define MEMORY_PROFILING_UNIT peak
// user defines

#ifdef MYGP
typedef struct _instruction{
	gchar* in;
	gint depth;
	gboolean fixed;
}instruction;

instruction* copyInstruction(instruction*);

void free_instruction(instruction*);

instruction* getData(GList*);

#endif // MYGP

typedef enum _gene{
	malloc_alignment,
	malloc_footer_insecure_no_segment_traversal,
	malloc_default_granularity,
	malloc_default_trim_threshold,
	malloc_default_mmap_threshold,
	malloc_max_release_check_rate,
	malloc_top_foot_size,
	malloc_line_4114,
	malloc_line_4353,
	malloc_line_4059,
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
	mutation_power2_allow0,
	mutation_boolean
}mutType;

typedef struct _individual{
	GList* ins;
	gint chrom[NUMBER_OF_GENE];
	double fitness;
	double time;
	double memory;
	double failNum;
	double time_repeat[REPEAT];
	double memory_repeat[REPEAT];
	gint paretoLevel;
	double crowdDistance;
	struct _individual* dominateSet[COMBINED_POPULATION_SIZE];
	gint dominateCount;
	gint beDominatedCount;
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

gint saveIndividual(individual*, gchar*);

void printIndividual(individual*);

void savePopulation(GList* , gint);

individual* copyIndividual(individual*);

void free_individual(individual*);

// compare two individuals according to their front level and crowd distance
// return 1 if the first one is better, -1 the second one is better, 0 the same
gint cmpIndividual(individual*, individual*);

// compare two files, returns 0 if identical, 1 otherwise
gint cmpOutput(gchar*, gchar*);

#endif // PUBLIC_H
