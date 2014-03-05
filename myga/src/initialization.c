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
			g_printf("\n");
			fprintf(logfp, "\n");
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
	calculateFrontier(&population);
	return population;
}
