#ifndef MUTATION_H
#define MUTATION_H

#include "public.h"

#define RATE_DELETE 0.6
#define RATE_INSERT 0.3
#define RATE_REPLACE (1-RATE_DELETE-RATE_INSERT)

gint mutationDelete(individual*);

#ifdef MYGP
gint mutationInsert(individual*, instruction*);

gint mutationReplace(individual*, instruction*);
#endif // MYGP

gint mutationGene(mutType, gint, gint, gint);

gint mutation(GList**);

#endif
