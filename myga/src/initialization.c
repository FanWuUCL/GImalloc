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
	population=g_list_append(population, ind);
	for(i=1; i<number; i++){
		ind=copyIndividual(seed);
		//random=randomIntRange(0, numberOfGenes);
		for(j=0; j<numberOfGenes; j++){
			ind->chrom[j]=mutationGene(default_mutation_type[j], ind->chrom[j], default_lower_bound[j], default_upper_bound[j]);
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
	population=g_list_append(population, ind);
	//gint printGap=number/100==0?1:number/100;
	for(i=1; i<number; i++){
		ind=copyIndividual(seed);
		if(i%50==0){
			g_printf("\n");
		}
		//random=randomIntRange(0, numberOfGenes);
		for(j=0; j<numberOfGenes; j++){
			ind->chrom[j]=mutationGene(default_mutation_type[j], ind->chrom[j], default_lower_bound[j], default_upper_bound[j]);
		}
		evaluateIndividual(ind, i);
		population=g_list_append(population, ind);
	}
	g_printf("\n");
	calculateFrontier(&population);
	return population;
}
