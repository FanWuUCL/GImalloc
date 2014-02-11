#ifndef EVALUATION_H
#define EVALUATION_H
#include "public.h"

// each time sleep SLEEP_UNIT/1,000,000 seconds
#define MAX_TIMEOUT_IN_SEC 5
#define MAX_TIMEOUT MAX_TIMEOUT_IN_SEC*1000000/SLEEP_UNIT

double evaluateIndividual(individual*, gint);

#endif
