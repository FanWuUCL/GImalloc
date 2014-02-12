#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
//#include <fcntl.h>
#include "profiler.h"

#define PIPE_IN 0 
#define PIPE_OUT 1

extern gint profiler_debug;

gint readProcMemory(gint i){
	gchar* filename=g_malloc(128*sizeof(gchar));
	gchar* line=g_malloc(128*sizeof(gchar));
	g_snprintf(filename, 128, "%sprocMemory%d", CURRDIR, i);
	if(!g_file_test(filename, G_FILE_TEST_EXISTS)){
		fprintf(stderr, "Warning, procMemory does not exist.\n");
		return 0;
	}
	FILE* fp=fopen(filename, "r");
	gint max=0, one=0, sum=0;
	while(!feof(fp)){
		if(!fgets(line, 128, fp)){
			break;
		}
		if(g_str_has_prefix(line, "memory:")){
			sscanf(line, "memory: %d", &one);
			sum+=one;
			if(sum>max) max=sum;
		}
	}
	g_free(filename);
	g_free(line);
	fclose(fp);
	return max;
}

gint profile(double* time, double* memory, double* correctness, int suiteSize){
	double time_t=0, memory_t=0, memory_one_case=0, correctness_t=0;
	gint memory_new=0, memory_new_one=0;
	gchar* line;
	gchar* filename=g_malloc(128*sizeof(gchar));
	gchar* append=g_malloc(32*sizeof(gchar));
	if(profile_times==0) g_snprintf(append, 32, ".s");
	else append[0]='\0';
	gint i, j, length=g_list_length(testcases);
	gint fullTest=1;
	if(suiteSize==0 || suiteSize>length){
		suiteSize=length;
	}
	else{
		suiteSize=length;
	}
	// clean the output files
	for(i=0; i<length; i++){
		g_snprintf(filename, 128, "curr/out%d%s", i, append);
		if(g_file_test(filename, G_FILE_TEST_EXISTS)){
			g_remove(filename);
		}
	}
	line=g_malloc(512*sizeof(gchar));
	gchar **cmdv=NULL;
	gint pid, mpid, sampleN;
	FILE *mfp;
	time_t=0;
	double tmp_double;
	gint tmp_int;
	struct rusage usage;
	for(i=0; i<suiteSize; i++){
		j=fullTest? i: randomIntRange(0, length);
		g_snprintf(filename, 128, "%sout%d%s", CURRDIR, j, append);
		sampleN=0;
		memory_one_case=0;
		g_snprintf(line, 512, "subject %s%s", TESTCASEDIR, (char*)g_list_nth_data(testcases, j));
		g_shell_parse_argv(line, NULL, &cmdv, NULL);
		if((pid=fork())<0){
			g_printf("fork() for subject execution failed.\n");
			exit(0);
		}
		if(pid==0){
			if(g_freopen(filename, "w", stdout)==NULL) perror("redirect to file failed.");
			g_snprintf(filename, 128, "%sprocMemory%d", CURRDIR, j);
			if(g_freopen(filename, "w", stderr)==NULL) perror("redirect stderr to file failed.");
			execv("./subject", cmdv);
			fprintf(logfp, "execv() failed.\n");
			fflush(logfp);
			exit(0);
		}
		g_strfreev(cmdv);

		// parent process, wait for pid
		wait4(pid, &tmp_int, 0, &usage);
		memory_new_one=readProcMemory(j);
		memory_new+=memory_new_one;
		time_t+=usage.ru_utime.tv_sec+usage.ru_stime.tv_sec+(usage.ru_utime.tv_usec+usage.ru_stime.tv_usec)/(double)1000000;
	}
	g_free(line);
	// cmp the output and summarise the correctness
	correctness_t=0;
	if(profile_times>0){
		for(i=0; i<length; i++){
			g_snprintf(filename, 128, "%sout%d", CURRDIR, i);
			line=g_list_nth_data(testcaseResults, i);
			correctness_t+=cmpOutput(filename, line);
		}
	}
	g_free(filename);
	g_free(append);
	if(profiler_debug) g_printf("memory: %lf\nmemory_new: %lf\ntime: %lf\n", memory_t, memory_new/(double)1024, time_t);
	*time=time_t;
	*memory=memory_new/(double)1024;
	//*correctness=correctness_t;
	*correctness=0;
	profile_times++;
	return 0;
}
