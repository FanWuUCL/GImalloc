#include "utility.h"
#include <math.h>

gint randomIntRange(gint a, gint b){
	return g_rand_int_range(randomUtility, a, b);
}

double randomOne(){
	return g_rand_double(randomUtility);
}

void updateAttr(memoryUsage* memory, memoryUsage* stdMemory, individual* program, gint div){
	double std, mut;
	std=stdMemory->timeSys+stdMemory->timeUsr;
	mut=memory->timeSys+memory->timeUsr;
	if(std<0.0001){
		std=0.0001;
		if(mut<0.0001) mut=0.0001;
	}
	if(mut<0.0001) program->time+=0.0001/div;
	else program->time+=mut/std/div;
	program->memory+=memory->MEMORY_PROFILING_UNIT/(stdMemory->MEMORY_PROFILING_UNIT<0.01? 0.01:stdMemory->MEMORY_PROFILING_UNIT)/div;
}
