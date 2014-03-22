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
#define PIPE_IN 0 
#define PIPE_OUT 1

void evaluate(double* time_usr, double* time_sys, double* memory, double* failNum){
	gint fd[2], nbytes, childpid;
	gchar readbuffer[128];
	gchar **cmdv=NULL;
	g_snprintf(readbuffer, 128, "memory %s %s %d %lf", CURRDIR, TESTCASEDIR, profile_times==0?1:0, timeout_sec);
	g_shell_parse_argv(readbuffer, NULL, &cmdv, NULL);
	pipe(fd);

	if((childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	if(childpid == 0)
	{
		/* Child process closes up input side of pipe */
		close(fd[0]);

		dup2(1, fd[1]);
		execv("./memory", cmdv);
		perror("exec");
		exit(0);
	}
	else{
		/* Parent process closes up output side of pipe */
		close(fd[1]);

		/* Read in a string from the pipe */
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		readbuffer[nbytes]='\0';
		sscanf(readbuffer, "%lf %lf %lf %lf", time_usr, time_sys, memory, failNum);
	}
}

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
	double time_usr, time_sys, memory, failNum;
	evaluate(&(program->time_usr), &(program->time_sys), &(program->memory), &(program->failNum));

	program->time=program->time_usr+program->time_sys;
	program->time_repeat[0]=program->time;
	program->memory_repeat[0]=program->memory;
	gint i;
	if(program->failNum>0){
		program->time_usr=1e10;
		program->time_sys=1e10;
		program->time=1e10;
		program->memory=1e10;
		g_printf("#");
		fprintf(logfp, "#");
		fflush(stdout);
	}
	else{
		g_printf(".");
		fprintf(logfp, ".");
		fflush(stdout);
		for(i=1; i<REPEAT; i++){
			//profile(&time_usr, &time_sys, &memory, &failNum, 0);
			evaluate(&time_usr, &time_sys, &memory, &failNum);
			program->time_repeat[i]=time_usr+time_sys;
			program->memory_repeat[i]=memory;
			program->time+=program->time_repeat[i];
			if(program->memory<memory) program->memory=memory;
		}
		program->time/=REPEAT;
	}
	profile_times++;
	g_free(filename);
	return 0;
}
