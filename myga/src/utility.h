#ifndef UTILITY_H
#define UTILITY_H

#include "public.h"

#define MEMORY_WEIGHT 4

GRand* randomUtility;

gint randomIntRange(gint, gint);

double randomOne();

void updateAttr(memoryUsage*, memoryUsage*, individual*, gint);

#endif
