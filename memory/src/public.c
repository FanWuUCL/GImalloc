#include "public.h"
#include<stdio.h>

#define SAVE_VERBOSE 0
#define VERBOSE_LENGTH 32

const gchar TESTCASEDIR[]=DEFAULT_TESTCASES_DIR;

const gchar CURRDIR[]=DEFAULT_CURR_DIR;

const gchar GCC_M32[]=DEFAULT_COMPILE_CMD;

const gint default_individual[]={2, 8, 64, 2048, 256, 4095};

const gint default_lower_bound[]={1, 0, 4, 64, 16, 1000};

const gint default_upper_bound[]={16, 15, 512, 16*1024, 2048, 10000};

const mutType default_mutation_type[]={mutation_power2, mutation_random, mutation_power2, mutation_power2, mutation_random, mutation_random};

gint saveIndividual(individual* program, gchar* filename){
	if(!filename) g_printf("Function saveIndividual(): filename is not valid.\n");
	//g_printf("Saving individual %s (address: %x)\n", filename, program);
	if(!program){
		g_printf("function saveIndividual(): *program is not valid.");
		return 0;
	}
	if(g_file_test(filename, G_FILE_TEST_EXISTS)){
		if(g_remove(filename)!=0){
			g_printf("remove outdated file failed.");
			return 0;
		}
	}
	gchar* cmd=g_malloc0(512*sizeof(gchar));
	g_snprintf(cmd, 512, "gcc %s -shared -o %s -O3 -fPIC malloc.c -D\'MALLOC_ALIGNMENT=((size_t)(%d*sizeof(void*)))\' -DFOOTERS=%d -DINSECURE=%d -DNO_SEGMENT_TRAVERSAL=%d -DMORECORE_CONTIGUOUS=%d -DDEFAULT_GRANULARITY=%d -DDEFAULT_TRIM_THRESHOLD=%d -DDEFAULT_MMAP_THRESHOLD=%d -DMAX_RELEASE_CHECK_RATE=%d", GCC_M32, filename,
		program->chrom[0],
		(program->chrom[1] & FOOTER_BIT)==0? 0:1,
		(program->chrom[1] & INSECURE_BIT)==0? 0:1,
		(program->chrom[1] & NO_SEGMENT_TRAVERSAL_BIT)==0? 0:1,
		(program->chrom[1] & MORECORE_CONTIGUOUS_BIT)==0? 0:1,
		program->chrom[2]*1024,
		program->chrom[3]*1024,
		program->chrom[4]*1024,
		program->chrom[5]);
	//g_spawn_command_line_sync(cmd, NULL, NULL, NULL, NULL);
	system(cmd);
	g_free(cmd);
	if(!g_file_test(filename, G_FILE_TEST_EXISTS)){
		return 0;
	}
	return 1;
}

void savePopulation(GList* population, gint generation){
	GList* p;
	individual* ind;
	gchar* filename=g_malloc0(64*sizeof(gchar));
	snprintf(filename, 64, "population");
	if(!g_file_test(filename, G_FILE_TEST_EXISTS)){
		g_mkdir(filename, S_IRWXU);
	}
	snprintf(filename, 64, "population/generation%d", generation);
	if(g_file_test(filename, G_FILE_TEST_EXISTS)){
		g_rmdir(filename);
	}
	g_mkdir(filename, S_IRWXU);
	p=population;
	gint index=0, i;
	snprintf(filename, 64, "population/generation%d/sum.txt", generation);
	FILE* fp=fopen(filename, "w+");
	while(p){
		ind=p->data;
		fprintf(fp, "m%d\t%lf\t%lf\t%lf\t%d\t%lf", index, ind->time, ind->memory, ind->failNum, ind->paretoLevel, ind->crowdDistance);
		for(i=0; i<NUMBER_OF_GENE; i++){
			fprintf(fp, "\t%d", ind->chrom[i]);
		}
		for(i=0; i<REPEAT; i++){
			fprintf(fp, "\t%lf", ind->time_repeat[i]);
		}
		fprintf(fp, "\t");
		for(i=0; i<REPEAT; i++){
			fprintf(fp, "\t%lf", ind->memory_repeat[i]);
		}
		fprintf(fp, "\n");
		p=p->next;
		index++;
	}
	p=stdMemoryAvg;
	memoryUsage *m;
	while(p){
		m=p->data;
		fprintf(fp, "%lf\t%lf\t%lf\n", m->timeUsr, m->MEMORY_PROFILING_UNIT, m->failNum);
		p=p->next;
	}
	fclose(fp);
	g_free(filename);
}

individual* copyIndividual(individual* ind){
	individual* copy=g_malloc0(sizeof(individual));
	copy->fitness=0;
	copy->ins=NULL;
	gint i;
	for(i=0; i<NUMBER_OF_GENE; i++){
		copy->chrom[i]=ind->chrom[i];
	}
	copy->time=0;
	copy->memory=0;
	copy->failNum=0;
	copy->paretoLevel=0;
	copy->crowdDistance=0;
	return copy;
}

void free_individual(individual* ind){
	g_free(ind);
}

gint cmpIndividual(individual* p, individual* q){
	if(p->paretoLevel<q->paretoLevel) return 1;
	if(p->paretoLevel>q->paretoLevel) return -1;
	if(p->crowdDistance>q->crowdDistance) return 1;
	if(p->crowdDistance<q->crowdDistance) return -1;
	return 0;
}

void printIndividual(individual* ind){
	gint i;
	g_printf("Chromosome: ");
	for(i=0; i<NUMBER_OF_GENE; i++){
		g_printf("%d\t", ind->chrom[i]);
	}
	g_printf("\nfitness:\t%lf", ind->fitness);
	g_printf("\ntime:\t%lf", ind->time);
	g_printf("\nmemory:\t%lf", ind->memory);
	g_printf("\nfail number:\t%lf", ind->failNum);
	g_printf("\nparetoLevel:\t%d", ind->paretoLevel);
	g_printf("\ncrowdDistance:\t%lf\n", ind->crowdDistance);
	
}

gint cmpOutput(gchar* filename1, gchar* filename2){
	FILE* fp1=fopen(filename1, "r");
	FILE* fp2=fopen(filename2, "r");
	if(!fp1 || !fp2){
		fprintf(logfp, "open camparison files fail.\n");
		return 1;
	}
	gint flag=1;
	gchar* line1=g_malloc0(256*sizeof(gchar));
	gchar* line2=g_malloc0(256*sizeof(gchar));
	while(!feof(fp1) && !feof(fp2)){
		flag=0;
		line1=fgets(line1, 256, fp1);
		line2=fgets(line2, 256, fp2);
		if(line1==NULL && line2==NULL) break;
		if(line1==NULL || line2==NULL){
			flag=1;
			break;
		}
		if(strcmp(line1, line2)){
			flag=1;
			//fprintf(logfp, "check point 3 in cmpOutput().\n");
			//fflush(logfp);
			break;
		}
	}
	if(line1) g_free(line1);
	if(line2) g_free(line2);
	fclose(fp1);
	fclose(fp2);
	return flag;
}

#ifdef MYGP
instruction* copyInstruction(instruction* i){
	instruction* copy=g_malloc0(sizeof(instruction));
	copy->depth=i->depth;
	copy->fixed=i->fixed;
	copy->in=g_strdup(i->in);
	return copy;
}

void free_instruction(instruction* a){
	if(a){
		if(a->in) g_free(a->in);
		g_free(a);
	}
}

instruction* getData(GList* glist){
	instruction* i=glist->data;
	return i;
}
#endif // MYGP
