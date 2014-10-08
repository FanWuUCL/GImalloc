#ifndef INITIALIZATION_H
#define INITIALIZATION_H
#include "public.h"

GList* initializePopulation(individual*, gint);

GList* initializeRandPopulation(individual*, gint);

GList* initializePopulationFromFile(individual*, gint, gchar*);

#endif
