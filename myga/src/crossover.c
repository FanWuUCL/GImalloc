#include "crossover.h"
#include "evaluation.h"
#include "selection.h"

// a and b should be the copies of the true parent individuals, children will cover a and b
gint crossoverTwoPoint(individual* a, individual* b){
	// determine crossover point
	gint begin=randomIntRange(0, numberOfGenes);
	gint end=randomIntRange(0, numberOfGenes);
	gint tmp;
	if(end<begin){
		tmp=begin;
		begin=end;
		end=tmp;
	}
	gint i;
	for(i=begin; i<=end; i++){
		tmp=a->chrom[i];
		a->chrom[i]=b->chrom[i];
		b->chrom[i]=tmp;
	}
	return 0;
}

gint crossoverScattered(individual* a, individual* b){
	gint tmp, i;
	for(i=0; i<numberOfGenes; i++){
		if(randomIntRange(0, 2)){
			tmp=a->chrom[i];
			a->chrom[i]=b->chrom[i];
			b->chrom[i]=tmp;
		}
	}
	return 0;
}

gint crossover(GList** population){
	gint num, length=g_list_length(*population);
	individual* parent1, *parent2, *child1, *child2;
	for(num=0; num<populationSize*crossoverRate; num+=2){
		//g_printf("crossover %d, size=%d\n", num+1, g_list_length(*population));
		parent1=tournamentSelect(*population);
		child1=copyIndividual(parent1);
		parent2=tournamentSelect(*population);
		while(parent1==parent2) parent2=tournamentSelect(*population);
		child2=copyIndividual(parent2);
		crossoverScattered(child1, child2);
		child1->fitness=evaluateIndividual(child1, length+num);
		child2->fitness=evaluateIndividual(child2, length+num+1);
		*population=g_list_append(*population, child1);
		*population=g_list_append(*population, child2);
	}
	return 0;
}
