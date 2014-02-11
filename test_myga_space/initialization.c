#include "mutation.h"
#include "utility.h"
#include "initialization.h"
#include "evaluation.h"
#include "selection.h"

GList* initializePopulation(individual* seed, gint number){
	GList* population=NULL;
	gint i;
	gint random;
	individual* ind;
	ind=copyIndividual(seed);
	evaluateIndividual(ind, 0);
	population=g_list_append(population, ind);
	for(i=1; i<number; i++){
		ind=copyIndividual(seed);
		random=randomIntRange(0, NUMBER_OF_GENE);
		ind->chrom[random]=mutationGene(default_mutation_type[random], ind->chrom[random], default_lower_bound[random], default_upper_bound[random]);
		evaluateIndividual(ind, i);
		population=g_list_append(population, ind);
	}
	calculateFrontier(&population);
	return population;
}
