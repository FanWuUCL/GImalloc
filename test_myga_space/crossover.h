#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "public.h"
#include "utility.h"

gint crossoverTwoPoint(individual*, individual*);

gint crossoverScattered(individual*, individual*);

gint crossover(GList**);

#endif
