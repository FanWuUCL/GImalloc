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
	gint num, length=g_list_length(*population), pos=0;
	individual* parent1, *parent2, *child1, *child2;
	mutType mt;
	/* re-evaluate the population from the last generation 
	GList* p=*population;
	while(p!=NULL){
		parent1=p->data;
		evaluateIndividual(parent1, pos++);
		p=p->next;
	}
*/
	for(num=0; num<populationSize*crossoverRate; num+=2){
		//g_printf("crossover %d, size=%d\n", num+1, g_list_length(*population));
		parent1=tournamentSelect(*population);
		child1=copyIndividual(parent1);
		parent2=tournamentSelect(*population);
		while(parent1==parent2) parent2=tournamentSelect(*population);
		child2=copyIndividual(parent2);
		crossoverScattered(child1, child2);
		if(randomOne()<mutationRate){
			pos=randomIntRange(0, numberOfGenes);
			if(randomOne()<0.9){
				if(randomOne()<0.5){
					if(default_mutation_type[pos]==mutation_power2){
						mt=mutation_power2_random;
					}
					else if(default_mutation_type[pos]==mutation_power2_allow0){
						mt=mutation_power2_allow0_random;
					}
					else mt=mutation_random;
				}
				else mt=default_mutation_type[pos];
				child1->chrom[pos]=mutationGene(mt, child1->chrom[pos], default_lower_bound[pos], default_upper_bound[pos]);
			}
			else{
				child1->chrom[pos]=default_individual[pos];
			}
		}
		if(randomOne()<mutationRate){
			pos=randomIntRange(0, numberOfGenes);
			if(randomOne()<0.9){
				if(randomOne()<0.5){
					if(default_mutation_type[pos]==mutation_power2){
						mt=mutation_power2_random;
					}
					else if(default_mutation_type[pos]==mutation_power2_allow0){
						mt=mutation_power2_allow0_random;
					}
					else mt=mutation_random;
				}
				else mt=default_mutation_type[pos];
				child2->chrom[pos]=mutationGene(mt, child2->chrom[pos], default_lower_bound[pos], default_upper_bound[pos]);
			}
			else{
				child2->chrom[pos]=default_individual[pos];
			}
		}
		evaluateIndividual(child1, length+num);
		evaluateIndividual(child2, length+num+1);
		*population=g_list_append(*population, child1);
		*population=g_list_append(*population, child2);
	}
	return 0;
}
