#include "mutation.h"
#include "utility.h"
#include "initialization.h"
#include "evaluation.h"
#include "selection.h"

GList* initializePopulation(individual* seed, gint number){
	GList* population=NULL;
	gint i, j;
	gint random;
	individual* ind;
	ind=copyIndividual(seed);
	evaluateIndividual(ind, 0);
	memoryUsage* memoryU=stdMemoryAvg->data;
	memoryU->timeUsr=ind->time;
	memoryU->MEMORY_PROFILING_UNIT=ind->memory;
	population=g_list_append(population, ind);
	mutType mt;
	for(i=1; i<number; i++){
		ind=copyIndividual(seed);
		//random=randomIntRange(0, numberOfGenes);
		for(j=0; j<numberOfGenes; j++){
			if(default_mutation_type[j]==mutation_power2){
				mt=mutation_power2_random;
			}
			else if(default_mutation_type[j]==mutation_power2_allow0){
				mt=mutation_power2_allow0_random;
			}
			else mt=mutation_random;
			ind->chrom[j]=mutationGene(mt, ind->chrom[j], default_lower_bound[j], default_upper_bound[j]);
		}
		evaluateIndividual(ind, i);
		population=g_list_append(population, ind);
	}
	calculateFrontier(&population);
	return population;
}

GList* initializeRandPopulation(individual* seed, gint number){
	GList* population=NULL;
	gint i, j;
	gint random;
	individual* ind;
	ind=copyIndividual(seed);
	evaluateIndividual(ind, 0);
	memoryUsage* memoryU=stdMemoryAvg->data;
	memoryU->timeUsr=ind->time;
	memoryU->MEMORY_PROFILING_UNIT=ind->memory;
	population=g_list_append(population, ind);
	//gint printGap=number/100==0?1:number/100;
	mutType mt;
	for(i=1; i<number; i++){
		ind=copyIndividual(seed);
		if(i%50==0){
			g_printf("\n%d-%d: ", i, i+50);
			fprintf(logfp, "\n%d-%d: ", i, i+50);
		}
		//random=randomIntRange(0, numberOfGenes);
		for(j=0; j<numberOfGenes; j++){
			if(default_mutation_type[j]==mutation_power2){
				mt=mutation_power2_random;
			}
			else if(default_mutation_type[j]==mutation_power2_allow0){
				mt=mutation_power2_allow0_random;
			}
			else mt=mutation_random;
			ind->chrom[j]=mutationGene(mt, ind->chrom[j], default_lower_bound[j], default_upper_bound[j]);
		}
		evaluateIndividual(ind, i);
		population=g_list_append(population, ind);
	}
	g_printf("\n");
	selection(&population);
	return population;
}

GList* initializePopulationFromFile(individual* seed, gint number, gchar* filename){
	GList* population=NULL;
	gint i, j;
	individual* ind;
	gchar* line=g_malloc(512*sizeof(gchar));
	FILE* fp=fopen(filename, "r");
	if(fp==NULL){
		g_printf("File %s does not exist.\n", filename);
		exit(0);
	}
	
	line=fgets(line, 512, fp);
	while(line!=NULL && line[0]=='m'){
		ind=copyIndividual(seed);
		if(numberOfGenes==NUMBER_OF_SHALLOW_GENE){
			sscanf(line, "%*s\t%*f\t%lf\t%*f\t%*d\t%*d\t%*f\t%*f\t%*f\t%d\t%d\t%d\t%d\t%d\t%d", 
				&ind->memory, &ind->chrom[0], &ind->chrom[1], &ind->chrom[2], &ind->chrom[3], &ind->chrom[4], &ind->chrom[5]);
		}
		else{
			sscanf(line, "%*s\t%*f\t%lf\t%*f\t%*d\t%*d\t%*f\t%*f\t%*f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", 
				&ind->memory, &ind->chrom[0], &ind->chrom[1], &ind->chrom[2], &ind->chrom[3], &ind->chrom[4], &ind->chrom[5],
				&ind->chrom[6], &ind->chrom[7], &ind->chrom[8], &ind->chrom[9], &ind->chrom[10], &ind->chrom[11], &ind->chrom[12], &ind->chrom[13], &ind->chrom[14]);
		}
		population=g_list_append(population, ind);
		line=fgets(line, 512, fp);
	}
	
	g_free(line);
	fclose(fp);
	return population;
}
