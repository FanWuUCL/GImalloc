#include "public.h"
#include "utility.h"
#include "evaluation.h"
#include "crossover.h"
#include "mutation.h"
#include "selection.h"
#include "initialization.h"
#include "profiler.h"
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<math.h>
#include<time.h>

extern individual* getOriginal();

extern memoryUsage* massifInterpreter(gchar*);

gint profiler_debug;

void readTestcases(){
	FILE* fp=fopen("testcases.txt", "r+");
	if(!fp){
		g_printf("Read Testcases fail.\n");
		return;
	}
	gchar* line;
	gint i;
	while(!feof(fp)){
		line=g_malloc0(128*sizeof(gchar));
		if(!fgets(line, 128, fp)){
			continue;
		}
		i=0;
		while(line[i]!='\n' && line[i]!='\0') i++;
		if(i==0) continue;
		if(line[i]=='\n') line[i]='\0';
		testcases=g_list_append(testcases, line);
	}
	fullTest=g_list_length(testcases)<=CASES_PER_PROG? 1:0;
	fclose(fp);
}

void readTestcaseResults(){
	gchar* line;
	gchar* tcase;
	gint i, length=g_list_length(testcases);
	for(i=0; i<length; i++){
		line=g_malloc0(128*sizeof(gchar));
		g_snprintf(line, 128, "%sout%d.s", CURRDIR, i);
		testcaseResults=g_list_append(testcaseResults, line);
	}
}

void readStdMemory(){
	readTestcaseResults();
	gchar* filename="libmalloc.so";
	if(!saveIndividual(ori, filename)){
		g_printf("Compiling original .so failed.\n");
		exit(0);
	}
	memoryUsage *stdM=g_malloc0(sizeof(memoryUsage));
	gint i;
	double time, memory, failNum;
	profile(&time, &memory, &failNum, 0);
	stdM->timeUsr=time;
	stdM->MEMORY_PROFILING_UNIT=memory;
	stdM->failNum=failNum;
	for(i=1; i<REPEAT; i++){
		profile(&time, &memory, &failNum, 0);
		stdM->timeUsr+=time;
		if(stdM->MEMORY_PROFILING_UNIT<memory) stdM->MEMORY_PROFILING_UNIT=memory;
	}
	stdM->timeUsr/=REPEAT;
	stdMemoryAvg=g_list_append(stdMemoryAvg, stdM);
}

void free_population(GList* population){
	GList* entry=population;
	individual* ind;
	while(entry){
		ind=entry->data;
		free_individual(ind);
		entry=entry->next;
	}
	g_list_free(population);
}

gint prepare(gchar* originalFile){
	ori=NULL;
	profiler_debug=0;
	profile_times=0;
	logfp=fopen("log.txt", "w+");
	ori=getOriginal(originalFile);
	randomUtility=g_rand_new();
	readTestcases();
	readStdMemory();
	return 0;
}

gint wrapup(){
	g_rand_free(randomUtility);
	g_list_free(testcases);
	g_list_free(testcaseResults);
	g_list_free(stdMemoryAvg);
	free_individual(ori);
	fclose(logfp);
	return 0;
}

static void printList(GList* list){
	gint i, length=g_list_length(list);
	g_printf("Checking List:\n");
	for(i=0; i<length; i++){
		g_printf("\t%s\n", (gchar*)g_list_nth_data(list, i));
	}
}

static void printStdComsumption(){
	gint i, length=g_list_length(stdMemoryAvg);
	memoryUsage* m;
	g_printf("Checking standard comsumpation List:\n");
	for(i=0; i<length; i++){
		m=g_list_nth_data(stdMemoryAvg, i);
		g_printf("\tUsr:%lf, Memory:%lf, failNum:%lf\n", m->timeUsr, m->MEMORY_PROFILING_UNIT, m->failNum);
	}
}

void main(int argc, char** argv){
	prepare(NULL);
	GList* population=NULL;
	//printList(testcases);
	//printList(testcaseResults);
	printStdComsumption();
	
	population=initializePopulation(ori, POPULATION_SIZE);
	
	int i;
	int j;
	time_t t1, t2;
	for(i=0; i<GENERATION_MAX; i++){
		t1=time(NULL);
		g_printf("========================GENERATION %d=========================\n", i);
		mutation(&population);
		crossover(&population);

		selection(&population);
		if(1){
			savePopulation(population, i);
		}
		t2=time(NULL);
		g_printf("Local time: %s\n", ctime(&t2));
		t1=(t2-t1)*(GENERATION_MAX-i-1)+t2;
		g_printf("Estimated finish time: %s\n", ctime(&t1));
	}

	savePopulation(population, 999);
	free_population(population);

	wrapup();
}
