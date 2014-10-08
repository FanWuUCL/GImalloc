#include "public.h"
#include<stdio.h>

#define SAVE_VERBOSE 0
#define VERBOSE_LENGTH 32

const gchar TESTCASEDIR[]=DEFAULT_TESTCASES_DIR;

const gchar CURRDIR[]=DEFAULT_CURR_DIR;

const gchar GCC_M32[]=DEFAULT_COMPILE_CMD;

const gint default_individual[]={DEFAULT_IND};

const gint default_lower_bound[]={LOWERBOUND_IND};

const gint default_upper_bound[]={UPPERBOUND_IND};

const mutType default_mutation_type[]={MUTATIONTYPE_IND};

gint saveIndividual(individual* program, gchar* filename, gint instrument){
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
	gchar* instrumentString;
	if(instrument==0){
		instrumentString="-DINSTRUMENT=0";
	}
	else{
		instrumentString="-DINSTRUMENT=1";
	}
	gchar* cmd=g_malloc0(800*sizeof(gchar));
	if(program==ori){
		g_snprintf(cmd, 800, DEFAULT_GCC_MALLOC, GCC_M32, filename, instrumentString);
	}
	else if(numberOfGenes==NUMBER_OF_GENE){
		g_snprintf(cmd, 800, DEEP_GCC_MALLOC, GCC_M32, filename, instrumentString,
			program->chrom[0],
			(program->chrom[1] & FOOTER_BIT)==0? 0:1,
			(program->chrom[1] & INSECURE_BIT)==0? 0:1,
			(program->chrom[1] & NO_SEGMENT_TRAVERSAL_BIT)==0? 0:1,
			(program->chrom[1] & MORECORE_CONTIGUOUS_BIT)==0? 0:1,
			program->chrom[2]*1024,
			program->chrom[3]*1024,
			program->chrom[4]*1024,
			program->chrom[5],
			program->chrom[6],
			program->chrom[7],
			program->chrom[8],
			program->chrom[9],
			program->chrom[10],
			program->chrom[11],
			program->chrom[12],
			program->chrom[13],
			program->chrom[14]);
	}
	else{
		g_snprintf(cmd, 800, SHALLOW_GCC_MALLOC, GCC_M32, filename, instrumentString,
			program->chrom[0],
			(program->chrom[1] & FOOTER_BIT)==0? 0:1,
			(program->chrom[1] & INSECURE_BIT)==0? 0:1,
			(program->chrom[1] & NO_SEGMENT_TRAVERSAL_BIT)==0? 0:1,
			(program->chrom[1] & MORECORE_CONTIGUOUS_BIT)==0? 0:1,
			program->chrom[2]*1024,
			program->chrom[3]*1024,
			program->chrom[4]*1024,
			program->chrom[5]);
	}
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
		fprintf(fp, "m%d\t%lf\t%lf\t%lf\t%d\t%d\t%lf\t%lf\t%lf", index, ind->time, ind->memory, ind->failNum, ind->evaluateTimes, ind->paretoLevel, ind->crowdDistance, ind->time_usr, ind->time_sys);
		for(i=0; i<numberOfGenes; i++){
			fprintf(fp, "\t%d", ind->chrom[i]);
		}
		for(i=0; i<REPEAT; i++){
			fprintf(fp, "\t%lf", ind->time_repeat[i]);
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

void saveSelectivePopulation(GList* population, gchar* file){
	GList* p;
	individual* ind;
	p=population;
	gint index=0, i;
	FILE* fp=fopen(file, "w+");
	while(p){
		ind=p->data;
		if(ind->paretoLevel==1){
			fprintf(fp, "m%d\t%lf\t%lf\t%lf\t%d\t%d\t%lf\t%lf\t%lf", index, ind->time, ind->memory, ind->failNum, ind->evaluateTimes, ind->paretoLevel, ind->crowdDistance, ind->time_usr, ind->time_sys);
			for(i=0; i<numberOfGenes; i++){
				fprintf(fp, "\t%d", ind->chrom[i]);
			}
			for(i=0; i<REPEAT; i++){
				fprintf(fp, "\t%lf", ind->time_repeat[i]);
			}
			fprintf(fp, "\n");
		}
		p=p->next;
		index++;
	}
	fprintf(fp, "ori\t%lf\t%lf\n", ori->time, ori->memory);
	fclose(fp);
}

individual* copyIndividual(individual* ind){
	individual* copy=g_malloc0(sizeof(individual));
	gint i;
	for(i=0; i<numberOfGenes; i++){
		copy->chrom[i]=ind->chrom[i];
	}
	copy->evaluateTimes=0;
	copy->time=0;
	copy->time_usr=0;
	copy->time_sys=0;
	copy->memory=0;
	copy->failNum=0;
	copy->paretoLevel=0;
	copy->crowdDistance=0;
	copy->dominateSet=(individual**)g_malloc0(COMBINED_POPULATION_SIZE*sizeof(individual*));
	return copy;
}

void free_individual(individual* ind){
	g_free(ind->dominateSet);
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
	for(i=0; i<numberOfGenes; i++){
		g_printf("%d\t", ind->chrom[i]);
	}
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
