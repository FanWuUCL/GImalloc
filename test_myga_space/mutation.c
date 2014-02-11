#include "mutation.h"
#include "evaluation.h"
#include "selection.h"
#include "utility.h"

gint mutationDelete(individual* a){
	gint length=g_list_length(a->ins);
	GList* pos;
	pos=g_list_nth(a->ins, randomIntRange(0, length));
	while(getData(pos)->fixed) pos=g_list_nth(a->ins, randomIntRange(0, length));
	instruction* in=pos->data;
	a->ins=g_list_remove(a->ins, in);
	free_instruction(in);
	return 0;
}

#ifdef MYGP
gint mutationInsert(individual* a, instruction* i){
	gint length=g_list_length(a->ins);
	GList* pos;
	pos=g_list_nth(a->ins, randomIntRange(0, length));
	while(getData(pos)->fixed) pos=g_list_nth(a->ins, randomIntRange(0, length));
	i->depth=getData(pos)->depth;
	i->fixed=FALSE;
	a->ins=g_list_insert(a->ins, i, g_list_index(a->ins, pos->data));
	return 0;
}

gint mutationReplace(individual* a, instruction* i){
	gint length=g_list_length(a->ins);
	GList* pos;
	pos=g_list_nth(a->ins, randomIntRange(0, length));
	while(getData(pos)->fixed) pos=g_list_nth(a->ins, randomIntRange(0, length));
	i->depth=getData(pos)->depth;
	i->fixed=getData(pos)->fixed;
	free_instruction(pos->data);
	pos->data=i;
	return 0;
}
#endif // MYGP

gint mutationGene(mutType mt, gint gene, gint lower, gint upper){
	gint maxGap, index;
	switch(mt){
		case mutation_one:		// mutation_one
			if(gene==lower) return gene+1;
			if(gene==upper) return gene-1;
			gene+=randomIntRange(0, 2)>0? 1:-1;
			break;
		case mutation_gap:		// mutation_gap
			maxGap=(upper-lower)/20;
			if(maxGap==0) maxGap=1;
			gene=gene+randomIntRange(0, 2*maxGap)-maxGap;
			if(gene<lower) gene=lower;
			else if(gene>upper) gene=upper;
			break;
		case mutation_random:		// mutation_random
			gene=randomIntRange(lower, upper+1);
			break;
		case mutation_power2:		// mutation_power2
			/*if(gene==lower) return gene*2;
			if(gene==upper) return gene/2;
			if(randomIntRange(0, 2)) gene*=2;
			else gene/=2;*/
			maxGap=upper/lower;
			index=0;
			while(maxGap>1){
				maxGap/=2;
				index++;
			}
			index=randomIntRange(0, index+1);
			for(gene=lower; index>0; index--){
				gene*=2;
			}
			break;
		case mutation_power2_allow0:		// mutation_power2_allow0
			maxGap=upper/lower;
			index=0;
			while(maxGap>1){
				maxGap/=2;
				index++;
			}
			maxGap=index;
			index=randomIntRange(0, maxGap+2);
			if(index==maxGap+1){
				gene=0;
				break;
			}
			for(gene=lower; index>0; index--){
				gene*=2;
			}
			break;
		case mutation_boolean:		// mutation_boolean
			if(gene==0) gene=1;
			else gene=0;
			break;
		default:
			g_printf("Code shouldn't be reached in mutationGene(), mutationType=%d\n", mt);
			exit(0);
	}
	return gene;
}

gint mutation(GList** population){
	gint num, length=g_list_length(*population);
	individual* ind, *copy;
	gint pos;
	for(num=0; num<POPULATION_SIZE*MUTATION_RATE; num++){
		g_printf("mutation %d, size=%d\n", num+1, g_list_length(*population));
		ind=tournamentSelect(*population);
		copy=copyIndividual(ind);
		pos=randomIntRange(0, NUMBER_OF_GENE);
		copy->chrom[pos]=mutationGene(default_mutation_type[pos], copy->chrom[pos], default_lower_bound[pos], default_upper_bound[pos]);
		copy->fitness=evaluateIndividual(copy, length+num);
		*population=g_list_append(*population, copy);
	}
	return 0;
}
