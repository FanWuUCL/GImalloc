// profile time, memory, correctness, ect.
#ifndef PROFILER_H
#define PROFILER_H

#include <signal.h>
#include "public.h"
#include "utility.h"

gint profile_times;

/* meaning of returned value
	0:	no error happened, and the results should be stored in the parameters.
	1:	opening testcases file failed.
*/
gint profile(double*, double*, double*, int);

#endif
