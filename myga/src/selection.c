#include "selection.h"
#include "evaluation.h"

GList* my_list_remove(GList* list, gpointer gp){
	GList* p=list;
	while(p && p->data!=gp){
		p=p->next;
	}
	if(p){
		if(p==list){
			//g_printf("!!!!!!!!! ATTENTION !!!!!!!!!!! List head %x is removed, ", list);
			list=p->next;
			list->prev=NULL;
			//p->next=NULL;
			//g_printf("new head is %x.\n", list);
		}
		else{
			p->prev->next=p->next;
			if(p->next) p->next->prev=p->prev;
			//p->next=NULL;
			//p->prev=NULL;
		}
	}
	return list;
}

static void crowding_distance_assignment(individual* front[], gint length){
	if(length==1){
		front[0]->crowdDistance=100;
		return;
	}
	gint i, j;
	individual* tmp;
	for(i=0; i<length-1; i++){
		for(j=1; j<length-i; j++){
			if(front[j]->time < front[j-1]->time){
				tmp=front[j];
				front[j]=front[j-1];
				front[j-1]=tmp;
			}
		}
	}
	front[0]->crowdDistance=front[length-1]->crowdDistance=100;
	for(i=1; i<length-1; i++){
		if(front[length-1]->time-front[0]->time==0) front[i]->crowdDistance=100;
		else 
			front[i]->crowdDistance+=(front[i+1]->time-front[i-1]->time)/(front[length-1]->time-front[0]->time);
	}
	// do the same procedure on the second objective
	for(i=0; i<length-1; i++){
		for(j=1; j<length-i; j++){
			if(front[j]->memory > front[j-1]->memory){
				tmp=front[j];
				front[j]=front[j-1];
				front[j-1]=tmp;
			}
		}
	}
	front[0]->crowdDistance=front[length-1]->crowdDistance=100;
	for(i=1; i<length-1; i++){
		if(front[length-1]->time-front[0]->time==0) front[i]->crowdDistance=100;
		else 
			front[i]->crowdDistance+=(front[i+1]->memory-front[i-1]->memory)/(front[length-1]->memory-front[0]->memory);
	}
	/* do the same procedure on the third objective */
#ifdef THIRD_DIMENSION_CORRECTNESS
	for(i=0; i<length-1; i++){
		for(j=1; j<length-i; j++){
			if(front[j]->failNum < front[j-1]->failNum){
				tmp=front[j];
				front[j]=front[j-1];
				front[j-1]=tmp;
			}
		}
	}
	front[0]->crowdDistance=front[length-1]->crowdDistance=100;
	for(i=1; i<length-1; i++){
		if(front[length-1]->time-front[0]->time==0) front[i]->crowdDistance=100;
		else 
			front[i]->crowdDistance+=(front[i+1]->memory-front[i-1]->memory)/(front[length-1]->memory-front[0]->memory);
	}
#endif
}

static gint dominate(individual* p, individual* q){
#ifdef THIRD_DIMENSION_CORRECTNESS
	if(p->time==q->time && p->memory==q->memory && p->failNum==q->failNum) return 0;
	if(p->time<=q->time && p->memory<=q->memory && p->failNum<=q->failNum) return 1;
	if(p->time>=q->time && p->memory>=q->memory && p->failNum>=q->failNum) return -1;
#else
	if(p->time==q->time && p->memory==q->memory) return 0;
	if(p->time<=q->time && p->memory<=q->memory) return 1;
	if(p->time>=q->time && p->memory>=q->memory) return -1;
#endif
	return 0;
}

void calculateFrontier(GList** population){
	gint i, j, length=g_list_length(*population);
	individual** front=(individual**)g_malloc0(COMBINED_POPULATION_SIZE*sizeof(individual*));
	individual* p, *q;
	gint frontLevel=0, tmpint, left=length;
	// determine the dominate set and being dominated count for each individual
	//g_printf("determine the dominate set and being dominated count for each individual.\n");
	for(i=0; i<length; i++){
		p=g_list_nth_data(*population, i);
		p->paretoLevel=0;
		p->crowdDistance=0;
		for(j=0; j<COMBINED_POPULATION_SIZE; j++){
			p->dominateSet[j]=NULL;
		}
		p->dominateCount=0;
		p->beDominatedCount=0;
		for(j=0; j<length; j++){
			q=g_list_nth_data(*population, j);
			tmpint=dominate(p, q);
			if(tmpint==1){
				p->dominateSet[p->dominateCount++]=q;
			}
			else if(tmpint==-1){
				p->beDominatedCount++;
			}
		}
		if(p->beDominatedCount==0){
			p->paretoLevel=1;
			// recording how many left haven't been assigned Pareto Level yet
			left--;
		}
	}
	// quick assign corresponding level to every individual
	//g_printf("quick assign corresponding level to every individual.\n");
	frontLevel=1;
	while(left>0){
		for(i=0; i<length; i++){
			p=g_list_nth_data(*population, i);
			if(p->paretoLevel!=frontLevel) continue;
			for(j=0; j<p->dominateCount; j++){
				q=p->dominateSet[j];
				q->beDominatedCount--;
				if(q->beDominatedCount==0){
					q->paretoLevel=frontLevel+1;
					left--;
				}
			}
			//g_printf("Individual %d:\n", i);
			//printIndividual(p);
		}
		frontLevel++;
	}
	// calculate the crowd distance for each front
	//g_printf("calculate the crowd distance for each front.\n");
	while(frontLevel>0){
		tmpint=0;
		for(i=0; i<length; i++){
			p=g_list_nth_data(*population, i);
			if(p->paretoLevel==frontLevel){
				front[tmpint++]=p;
			}
		}
		crowding_distance_assignment(front, tmpint);
		frontLevel--;
	}
}

void selection(GList** population){
	gint i, j, length;
	individual* ind, *worst;
	//g_printf("selection..., size=%d\n", g_list_length(*population));
	// if the testcase set is sampled during the evaluation, the parent population should be re-evaluated due to fairness
/*
	if(!fullTest){
		for(i=0; i<populationSize; i++){
			g_printf("selection---getting %dth/%d data\n", i, g_list_length(*population));
			ind=g_list_nth_data(*population, i);
			g_printf("selection---evaluating %dth data\n", i);
			ind->fitness=evaluateIndividual(ind, i);
		}
		g_printf("selection---original %d individual evaluated\n", populationSize);
	}
*/
	// selection
	// re-calculate the frontiers
	calculateFrontier(population);
	//g_printf("Finished frontier calculation for the extended population.\n");
	GList* p, *q;
	i=0, j=1;
	// determine the first how many frontiers suffice the population size
	while(i<populationSize){
		p=*population;
		while(p){
			ind=p->data;
			if(ind->paretoLevel==j){
				i++;
			}
			p=p->next;
		}
		j++;
	}
	// discard all the individuals in other frontiers
	//g_printf("The individual with Pareto Level bigger than %d will be removed.\n", j-1);
	p=*population;
	while(p){
		q=p->next;
		ind=p->data;
		if(ind->paretoLevel>=j){
			*population=g_list_remove(*population, ind);
			free_individual(ind);
		}
		p=q;
	}
	// discard the most crowded individuals in the last frontier among the frontiers left
	length=g_list_length(*population);
	j--;
	//g_printf("Removed all individuals whose Pareto level is bigger than %d, current size=%d\n", j, length);
	while(length>populationSize){
		worst=NULL;
		p=*population;
		while(p){
			ind=p->data;
			if(ind->paretoLevel==j){
				if(worst==NULL) worst=ind;
				else if(cmpIndividual(worst, ind)>0) worst=ind;
			}
			p=p->next;
		}
		*population=g_list_remove(*population, worst);
		free_individual(worst);
		length--;
	}
	// recalculate the frontiers again
	calculateFrontier(population);
	//g_printf("selection---population truncated, size=%d\n", g_list_length(*population));
}

individual* tournamentSelect(GList* population){
	gint size=g_list_length(population);
	gint i;
	individual* candidate=g_list_nth_data(population, randomIntRange(0, size));
	individual* comp;
	for(i=1; i<TOURNAMENT_SIZE; i++){
		comp=g_list_nth_data(population, randomIntRange(0, size));
		if(cmpIndividual(comp, candidate)>0) candidate=comp;
	}
	return candidate;
}

// evaluate every individual VERIFY_TIMES times
void verifyPopulation(GList** population){
	gint num, j, length=g_list_length(*population), pos=0;
	individual* ind;
	double accTime=0, accMemory=0;
	g_printf("Verifying original:\t");
	for(j=0; j<VERIFY_TIMES; j++){
		evaluateIndividual(ori, 0);
		accTime+=ori->time;
		accMemory+=ori->memory;
	}
	g_printf("\n");
	ori->time=accTime/VERIFY_TIMES;
	ori->memory=accMemory/VERIFY_TIMES;
	gint failTimes;
	for(num=0; num<length; num++){
		accTime=0;	
		accMemory=0;
		failTimes=0;
		ind=(individual*)g_list_nth_data(*population, num);
		g_printf("Verifying %d/%d:\t", num+1, length);
		for(j=0; j<VERIFY_TIMES; j++){
			evaluateIndividual(ind, num);
			if(ind->time>1000){
				failTimes++;
			}
			else{
				accTime+=ind->time;
				accMemory+=ind->memory;
			}
		}
		g_printf("\n");
		ind->time=accTime/(VERIFY_TIMES-failTimes);
		ind->memory=accMemory/(VERIFY_TIMES-failTimes);
	}
	calculateFrontier(population);
}
