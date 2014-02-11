#include "public.h"

individual* getOriginal(gchar* filename){
	if(ori) free_individual(ori);
	ori=g_malloc0(sizeof(individual));
	ori->fitness=0;
	gint i;
	for(i=0; i<NUMBER_OF_GENE; i++){
		ori->chrom[i]=default_individual[i];
	}
	return ori;
}
