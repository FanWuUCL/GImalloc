#ifndef SELECTION_H
#define SELECTION_H

#include "public.h"

#define TOURNAMENT_SIZE 5

void selection(GList**);

individual* tournamentSelect(GList*);

void calculateFrontier(GList**);

#endif
