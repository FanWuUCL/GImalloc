#include "public.h"

individual* getOriginal(){
	if(ori) free_individual(ori);
	ori=g_malloc0(sizeof(individual));
	ori->dominateSet=(individual**)g_malloc0(COMBINED_POPULATION_SIZE*sizeof(individual*));
	gint i;
	for(i=0; i<numberOfGenes; i++){
		ori->chrom[i]=default_individual[i];
	}
	ori->evaluateTimes=0;
	return ori;
}
