#include<glib.h>
#include<glib/gstdio.h>
#include<stdio.h>
#include<stdlib.h>

#define FILENAME "fitness.txt"
#define COMBINED_POPULATION_SIZE 5000
#define NUMBER_DESIRED 50
#define MAX_LINE_NUMBER 10000

gint instrLine[]={0, 3960, 
3961, 
3962, 
3964, 
3965, 
4135, 
4136, 
4185, 
4186, 
4369, 
4372, 
4374, 
4375, 
4376, 
4377, 
4378, 
4379, 
4381, 
4391, 
4398, 
4400, 
4401, 
4403, 
4425, 
4432, 
4434, 
4435, 
4437, 
4475, 
4478, 
4480, 
4481, 
4483, 
4490, 
4493, 
4495, 
4496, 
4498, 
4527, 
4529, 
4539, 
4543, 
4545, 
4546, 
4548, 
4709, 
4710, 
4769, 
4771, 
4783, 
4789, 
4791, 
4792, 
4794, 
4844, 
4845, 
5265, 
5266};

gint filter4335[]={936, 937, 935, 940, 911, 913};

gint isInstr(gint l){
	gint i;
	for(i=0; i<59; i++){
		if(l==instrLine[i]){
			return 1;
		}
	}
	return 0;
}

gint shouldFilter(gint lineNumber, gint mutantIndex){
	gint i;
	if(lineNumber==4335){
		for(i=0; i<6; i++){
			if(mutantIndex==filter4335[i])
				return 1;
		}
	}
	return 0;
}

typedef struct _individual{
	gint line;
	double time;
	double memory;
	gint nm;
	gint m[COMBINED_POPULATION_SIZE];
	gint paretoLevel;
	double crowdDistance;
	struct _individual *dominateSet[COMBINED_POPULATION_SIZE];
	gint dominateCount;
	gint beDominatedCount;
} individual;

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
		else front[i]->crowdDistance+=(front[i+1]->time-front[i-1]->time)/(front[length-1]->time-front[0]->time);
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
		else front[i]->crowdDistance+=(front[i+1]->memory-front[i-1]->memory)/(front[length-1]->memory-front[0]->memory);
	}
}

static gint dominate(individual* p, individual* q){
	if(p->time==q->time && p->memory==q->memory) return 0;
	if(p->time<=q->time && p->memory<=q->memory) return 1;
	if(p->time>=q->time && p->memory>=q->memory) return -1;
	return 0;
}

void calculateFrontier(GList** population){
	gint i, j, length=g_list_length(*population);
	individual* front[COMBINED_POPULATION_SIZE]={0};
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

gint cmpIndividual(individual* p, individual* q){
	if(p->paretoLevel<q->paretoLevel) return 1;
	if(p->paretoLevel>q->paretoLevel) return -1;
	if(p->crowdDistance>q->crowdDistance) return 1;
	if(p->crowdDistance<q->crowdDistance) return -1;
	return 0;
}

void main(){
	FILE* fp=fopen(FILENAME, "r");
	if(!fp){
		g_printf("Read fitness file failed.\n");
		return;
	}
	double defaultTime;
	double defaultMemory;
	gint i, j;
	fscanf(fp, "%*[^ ] %d %lf %lf", &i, &defaultMemory, &defaultTime);
	//printf("default memory %lf, default time %lf\n", defaultMemory, defaultTime);
	GList *combine=NULL, *p, *q;
	double time, memory;
	gint mutantIndex, lineNumber;
	gint biggestLineNumber=0;
	individual *ind;
	i=0;
	while(!feof(fp)){
		if(fscanf(fp, "%*[^0-9]%d %d %lf %lf", &mutantIndex, &lineNumber, &memory, &time)<3){
			continue;
		}
		memory=memory-defaultMemory;
		time=time-defaultTime;
		if(memory<-0.5*defaultMemory){
			continue;
		}
		if(isInstr(lineNumber)){
			continue;
		}
		p=NULL;
		/*p=combine;
		while(p!=NULL){
			ind=p->data;
			if(ind->line==lineNumber){
				break;
			}
			p=p->next;
		}*/
		if(p!=NULL){
			ind->m[ind->nm++]=mutantIndex;
			ind->time+=time;
			ind->memory+=memory;
		}
		else{
			ind=(individual*)g_malloc0(sizeof(individual));
			ind->line=lineNumber;
			ind->memory=memory;
			ind->time=time;
			ind->nm=1;
			ind->m[0]=mutantIndex;
			if(biggestLineNumber<lineNumber) biggestLineNumber=lineNumber;
			combine=g_list_append(combine, ind);
			i++;
		}
		//i++;
	}
	fclose(fp);
	fprintf(stderr, "Total %d entries recorded with biggest line number of %d.\n", i, biggestLineNumber);
	if(MAX_LINE_NUMBER<=biggestLineNumber){
		fprintf(stderr, "not enough space for line recorder: %d > %d\n", biggestLineNumber, MAX_LINE_NUMBER);
		return;
	}

	gint lineRecorder[MAX_LINE_NUMBER];
	for(i=0; i<MAX_LINE_NUMBER; i++){
		lineRecorder[i]=0;
	}

/*	checking whether reading file is successful.
	p=combine;
	while(p!=NULL){
		ind=p->data;
		printf("line %d, time %lf, memory %lf, ", ind->line, ind->time, ind->memory);
		for(j=0; j<ind->nm; j++){
			printf("%d ", ind->m[j]);
		}
		printf("\n");
		p=p->next;
	}
*/
/* average 
	p=combine;
	while(p!=NULL){
		ind=p->data;
		ind->time/=ind->nm;
		ind->memory/=ind->nm;
		p=p->next;
	}
*/
	calculateFrontier(&combine);
/*	check pareto level 
	p=combine;
	while(p!=NULL){
		ind=p->data;
		printf("line %d, time %lf, memory %lf, ", ind->line, ind->time, ind->memory);
		for(j=0; j<ind->nm; j++){
			printf("%d ", ind->m[j]);
		}
		printf(", pareto level %d, cd %lf", ind->paretoLevel, ind->crowdDistance);
		printf("\n");
		p=p->next;
	}
*/
	i=0, j=1;
	gint populationSize=NUMBER_DESIRED;
/* get first NUMBER_DESIRED mutants regardless of duplicated line number
	// determine the first how many frontiers suffice the population size
	while(i<populationSize){
		p=combine;
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
	p=combine;
	while(p){
		q=p->next;
		ind=p->data;
		if(ind->paretoLevel>=j){
			combine=g_list_remove(combine, ind);
			free(ind);
		}
		p=q;
	}
	// discard the most crowded individuals in the last frontier among the frontiers left
	gint length=g_list_length(combine);
	j--;
	gint lastLevel=j;
	individual* worst;
	//g_printf("Removed all individuals whose Pareto level is bigger than %d, current size=%d\n", j, length);
	while(length>populationSize){
		worst=NULL;
		p=combine;
		while(p){
			ind=p->data;
			if(ind->paretoLevel==j){
				if(worst==NULL) worst=ind;
				else if(cmpIndividual(worst, ind)>0) worst=ind;
			}
			p=p->next;
		}
		combine=g_list_remove(combine, worst);
		free(worst);
		length--;
	}
*/

/* get first NUMBER_DESIRE mutants with distinct line number */


	/* print out result */
	i=1;
	//while(i<=lastLevel){
	while(populationSize>0){
		p=combine;
		while(p!=NULL){
			ind=p->data;
			if(ind->paretoLevel==i && !lineRecorder[ind->line]){
				lineRecorder[ind->line]=1;
				printf("line %d, time %lf, memory %lf, ", ind->line, ind->time, ind->memory);
				for(j=0; j<ind->nm; j++){
					printf("%d ", ind->m[j]);
				}
				printf(", pareto level %d, cd %lf", ind->paretoLevel, ind->crowdDistance);
				printf("\n");
				populationSize--;
			}
			p=p->next;
		}
		i++;
	}
}

