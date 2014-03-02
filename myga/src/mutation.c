#include "mutation.h"
#include "evaluation.h"
#include "selection.h"
#include "utility.h"

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
	for(num=0; num<populationSize*mutationRate; num++){
		//g_printf("mutation %d, size=%d\n", num+1, g_list_length(*population));
		ind=tournamentSelect(*population);
		copy=copyIndividual(ind);
		pos=randomIntRange(0, numberOfGenes);
		copy->chrom[pos]=mutationGene(default_mutation_type[pos], copy->chrom[pos], default_lower_bound[pos], default_upper_bound[pos]);
		copy->fitness=evaluateIndividual(copy, length+num);
		*population=g_list_append(*population, copy);
	}
	return 0;
}
