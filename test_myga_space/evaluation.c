#include "evaluation.h"
#include "utility.h"
#include "profiler.h"
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<fcntl.h>
#include<errno.h>

double evaluateIndividual(individual* program, gint index){
	program->time=-1;
	program->memory=-1;
	program->failNum=-1;
	gchar* filename=g_malloc0(32*sizeof(gchar));
	g_snprintf(filename, 32, "libmalloc.so");
	if(!saveIndividual(program, filename)){
		g_free(filename);
		g_printf("save individual in evaluateIndividual() fail.\n");
		return 0;
	}
	double time, memory, failNum;
	profile(&(program->time), &(program->memory), &(program->failNum), 0);
	program->time_repeat[0]=program->time;
	program->memory_repeat[0]=program->memory;
	gint i;
	for(i=1; i<REPEAT; i++){
		profile(&time, &memory, &failNum, 0);
		program->time_repeat[i]=time;
		program->memory_repeat[i]=memory;
		program->time+=time;
		if(program->memory<memory) program->memory=memory;
	}
	program->time/=REPEAT;
	/*
	gint stdoutCopy, newStdout, redirectPipe[2];
	if(pipe(redirectPipe)!=0){
		fprintf(logfp, "generate pipe failed.\n");
		fflush(logfp);
		g_free(filename);
		return -1;
	}
	
	stdoutCopy=dup(STDERR_FILENO);
	newStdout=dup2(redirectPipe[PIPE_STDOUT], STDERR_FILENO);
	int pipeInFlag=fcntl(redirectPipe[PIPE_STDIN], F_GETFL, 0);
	pipeInFlag|=O_NONBLOCK;
	if(fcntl(redirectPipe[PIPE_STDIN], F_SETFL, pipeInFlag)<0){
		fprintf(logfp, "fcntl pipe in failed.\n");
		fflush(logfp);
		g_free(filename);
		return -1;
	}

	gchar* tcase;
	gchar* tresult;
	gchar* cmd=g_malloc0(512*sizeof(gchar));
	gchar* pipeout=g_malloc0(128*sizeof(gchar));
	gint numberOfCases=g_list_length(testcases);
	gint cases_per_prog=fullTest? numberOfCases:CASES_PER_PROG;
	gchar **argvForProc=NULL;
	GPid* pid=g_malloc0(sizeof(GPid));
	gint i, j, k, timeout, tmp;
	memoryUsage* memoryU=g_malloc0(sizeof(memoryUsage));
	memoryUsage* memoryU2=g_malloc0(sizeof(memoryUsage));
	memoryUsage* stdM;
	struct timeval start, end;
	double time_second;
	double old_time=program->time; program->time=0;
	double old_memory=program->memory; program->memory=0;
	program->passNumber=0;
	for(i=0; i<REPEAT; i++){
		program->time_repeat[i]=0;
		program->memory_repeat[i]=0;
	}
	for(i=0; i<cases_per_prog; i++){
		j=fullTest? i:randomIntRange(0, numberOfCases);
		g_snprintf(filename, 32, "%sout%d\0", CURRDIR, j);
		if(g_file_test(filename, G_FILE_TEST_EXISTS)){
			if(g_remove(filename)!=0){
				g_free(pid);
				g_free(filename);
				g_free(cmd);
				g_free(pipeout);
				return -2;
			}
		}
		freopen(filename, "w", stdout);
		tcase=g_list_nth_data(testcases, j);
		tresult=g_list_nth_data(testcaseResults, j);
		stdM=g_list_nth_data(stdMemoryAvg, j);
		g_snprintf(cmd, 512, "/usr/bin/time  -f \"TIME@ %%S %%U %%M\" ./subject %s%s", TESTCASEDIR, tcase);
		g_shell_parse_argv (cmd, NULL, &argvForProc, NULL);
		gettimeofday(&start,NULL);
		if(!g_spawn_async(NULL, argvForProc, NULL, G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, pid, NULL)){
			continue;
		}
		timeout=0;
		while(timeout<MAX_TIMEOUT && waitpid(*pid, NULL, WNOHANG)==0){
			timeout++;
			usleep(SLEEP_UNIT);
		}
		gettimeofday(&end,NULL);
		time_second=end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/(double)1000000;
		g_spawn_close_pid(*pid);
		if(timeout>=MAX_TIMEOUT) continue;
		tmp=read(redirectPipe[PIPE_STDIN], pipeout, 128);
		if(tmp<=0){
			fprintf(logfp, "Problem with profiling mutant process time and memory. continue.\n");
			fflush(logfp);
			continue;
		}
		pipeout[tmp]='\0';
		sscanf(pipeout, "%*[^@]@ %lf %lf %lf", &(memoryU->timeSys), &(memoryU->timeUsr), &(memoryU->MEMORY_PROFILING_UNIT));

		if(time_second-(memoryU->timeSys+memoryU->timeUsr)<0.1){
			memoryU->timeSys=0;
			memoryU->timeUsr=time_second;
		}
		program->time_repeat[0]+=(memoryU->timeSys+memoryU->timeUsr)/(stdM->timeSys+stdM->timeUsr);
		program->memory_repeat[0]+=memoryU->MEMORY_PROFILING_UNIT/stdM->MEMORY_PROFILING_UNIT;
		//fprintf(logfp, "Profiling m%d on testcase %d, sysTime=%lf, usrTime=%lf, memory=%lf\n", index, j, memoryU->timeSys, memoryU->timeUsr, memoryU->MEMORY_PROFILING_UNIT);
		//fflush(logfp);
		if(cmpOutput(tresult, filename)==0){
			if(memoryU->timeSys<0 || memoryU->timeUsr<0 || memoryU->MEMORY_PROFILING_UNIT<0) continue;
			program->passNumber++;
			for(k=1; k<REPEAT; k++){
				gettimeofday(&start,NULL);
				if(!g_spawn_async(NULL, argvForProc, NULL, G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, pid, NULL)){
					fprintf(logfp, "Running subject program fail in evaluation().\n");
					fflush(logfp);
					exit(0);
				}
				timeout=0;
				while(waitpid(*pid, NULL, WNOHANG)==0){
					timeout++;
					usleep(SLEEP_UNIT);
				}
				gettimeofday(&end,NULL);
				time_second=end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/(double)1000000;
				g_spawn_close_pid(*pid);
				tmp=read(redirectPipe[PIPE_STDIN], pipeout, 128);
				pipeout[tmp]='\0';
				sscanf(pipeout, "%*[^@]@ %lf %lf %lf", &(memoryU2->timeSys), &(memoryU2->timeUsr), &(memoryU2->MEMORY_PROFILING_UNIT));

				if(time_second-(memoryU2->timeSys+memoryU2->timeUsr)<0.1){
					memoryU2->timeSys=0;
					memoryU2->timeUsr=time_second;
				}
				memoryU->timeSys+=memoryU2->timeSys;
				memoryU->timeUsr+=memoryU2->timeUsr;
				memoryU->MEMORY_PROFILING_UNIT+=memoryU2->MEMORY_PROFILING_UNIT;
				program->time_repeat[k]+=(memoryU2->timeSys+memoryU2->timeUsr)/(stdM->timeSys+stdM->timeUsr);
				program->memory_repeat[k]+=memoryU2->MEMORY_PROFILING_UNIT/stdM->MEMORY_PROFILING_UNIT;
			}
			g_strfreev(argvForProc);
			argvForProc=NULL;
			memoryU->timeSys/=REPEAT;
			memoryU->timeUsr/=REPEAT;
			memoryU->MEMORY_PROFILING_UNIT/=REPEAT;
			updateAttr(memoryU, stdM, program, 1);
		}
		else{
			fprintf(logfp, "Individual %d has a diff output on case %d\n", index, j);
			fflush(logfp);
			program->time+=10000;
			program->memory+=10000;
			g_strfreev(argvForProc);
			argvForProc=NULL;
			continue;
		}
	}
	program->time/=cases_per_prog;
	program->memory/=cases_per_prog;
	if(old_time>0 && old_memory>0){
		program->time=FITNESS_TRANSIT_RATIO*old_time+(1-FITNESS_TRANSIT_RATIO)*program->time;
		program->memory=FITNESS_TRANSIT_RATIO*old_memory+(1-FITNESS_TRANSIT_RATIO)*program->memory;
	}
	for(i=0; i<REPEAT; i++){
		program->time_repeat[i]/=cases_per_prog;
		program->memory_repeat[i]/=cases_per_prog;
	}
	freopen("/dev/tty", "w", stdout);
	dup2(stdoutCopy, newStdout);
	close(stdoutCopy);
	close(redirectPipe[0]);
	close(redirectPipe[1]);
	g_free(memoryU);
	g_free(memoryU2);
	g_free(pid);
	g_free(cmd);
	g_free(pipeout);
*/
	g_free(filename);
	return 0;
}
