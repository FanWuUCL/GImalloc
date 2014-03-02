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
	if(stdM->timeUsr*TIMEOUT_MULTIPLE>timeout_sec){
		timeout_sec=(gint)stdM->timeUsr*TIMEOUT_MULTIPLE;
	}
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
	ori=getOriginal();
	randomUtility=g_rand_new();
	if(!g_file_test(CURRDIR, G_FILE_TEST_IS_DIR)){
		g_mkdir(CURRDIR, S_IRWXU);
	}
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

static void printManual(){
	g_printf("Format: myga [option value]\n");
	g_printf("options:\n");
	g_printf("\t-p\tpopulation size, default 50\n");
	g_printf("\t-g\tmaximum generation, default 100\n");
	g_printf("\t-t\ttimeout in seconds, default 5 sec\n");
	g_printf("\t-m\tmutation rate, default 0.2\n");
	g_printf("\t-c\tcrossover rate, not necessary equals to (1-m), default 0.8\n");
	g_printf("\t-d\twhether include deep parameters, 0 means don't include whilst 1 include, default 1\n");
	g_printf("\t-r\truning random search, 1 means using random search whilst 0 using NSGA II, default 0\n");
	g_printf("\t-h\tprint this manual\n");
}

static void parseArgs(gint argc, gchar** argv){
	populationSize=POPULATION_SIZE;
	generationMax=GENERATION_MAX;
	mutationRate=MUTATION_RATE;
	crossoverRate=CROSSOVER_RATE;
	numberOfGenes=NUMBER_OF_GENE;
	timeout_sec=DEFAULT_TIMEOUT_SEC;
	randomSearch=0;
	gint i=1;
	while(i<argc){
		if(g_strcmp0(argv[i], "-p")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			populationSize=atoi(argv[i+1]);
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-g")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			generationMax=atoi(argv[i+1]);
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-t")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			timeout_sec=atoi(argv[i+1]);
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-m")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			mutationRate=atof(argv[i+1]);
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-c")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			crossoverRate=atof(argv[i+1]);
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-d")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			if(atoi(argv[i+1])==0){
				numberOfGenes=NUMBER_OF_SHALLOW_GENE;
			}
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-r")==0){
			if(i+1>=argc){
				g_printf("Lack of value for %s\n", argv[i]);
				exit(0);
			}
			if(atoi(argv[i+1])==1){
				randomSearch=1;
			}
			i+=2;
		}
		else if(g_strcmp0(argv[i], "-h")==0){
			printManual();
			exit(0);
		}
		else{
			g_printf("Can't recognize option %s\n", argv[i]);
			printManual();
			exit(0);
		}
	}
	if(randomSearch==0){
		COMBINED_POPULATION_SIZE=(gint)(populationSize*(1+mutationRate+crossoverRate)+1);
	}
	else{
		COMBINED_POPULATION_SIZE=populationSize*generationMax;
		numberOfGenes=NUMBER_OF_SHALLOW_GENE;
	}
}

void main(int argc, char** argv){
	parseArgs(argc, argv);
	prepare(NULL);
	GList* population=NULL;

	printStdComsumption();

	if(randomSearch==1){
		g_printf("Random search with %d evaluations.\n", populationSize*generationMax);
		population=initializeRandPopulation(ori, populationSize*generationMax);
	}
	else{
		population=initializePopulation(ori, populationSize);
	
		int i;
		int j;
		time_t t1, t2;
		for(i=0; i<generationMax; i++){
			t1=time(NULL);
			g_printf("\n========================GENERATION %d=========================\n", i);
			mutation(&population);
			crossover(&population);

			selection(&population);
			if(1){
				savePopulation(population, i);
			}
			t2=time(NULL);
			g_printf("\nLocal time: %s", ctime(&t2));
			t1=(t2-t1)*(generationMax-i-1)+t2;
			g_printf("Estimated finish time: %s", ctime(&t1));
		}
	}
	savePopulation(population, 999);
	free_population(population);

	wrapup();
}
