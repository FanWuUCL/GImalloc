#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
//#include <fcntl.h>
#include "profiler.h"

#define PIPE_IN 0 
#define PIPE_OUT 1

#define TIME_OUT_SEC 3

extern gint profiler_debug;

gint profile(double* time, double* memory, double* correctness, int suiteSize){
	double time_t=0, memory_t=0, memory_one_case=0, correctness_t=0;
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
		g_snprintf(filename, 128, "out%d%s", i, append);
		if(g_file_test(filename, G_FILE_TEST_EXISTS)){
			g_remove(filename);
		}
	}
	gint redirectPipe[2];
	line=g_malloc(512*sizeof(gchar));
	gchar **cmdv=NULL;
	gint pid, mpid, sampleN;
	FILE *mfp;
	memory_t=0;
	time_t=0;
	double tmp_double;
	gint tmp_int;
	pipe(redirectPipe);
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
			if(g_freopen("/dev/null", "w", stderr)==NULL) perror("redirect stderr to file failed.");
			execv("./subject", cmdv);
			fprintf(logfp, "execv() failed.\n");
			fflush(logfp);
			exit(0);
		}
		g_strfreev(cmdv);
		g_snprintf(filename, 128, "/proc/%d/smaps", pid);
		if((mpid=fork())<0){
			fprintf(stderr, "fork() for memory sampling failed.\n");
			exit(0);
		}
		if(mpid==0){	// child process measuring the memory consumption
			//close(redirectPipe[PIPE_IN]);
			while(1){
				if(!g_file_test(filename, G_FILE_TEST_EXISTS)){
					//fprintf(logfp, "file %s not exist.\n", filename);
					break;
				}
				tmp_double=0;

				mfp=fopen(filename, "r");
				if(!mfp) break;
				while(!feof(mfp)){
					if(!fgets(line, 256, mfp)){
						break;
					}
					// search for writable mapping
					tmp_int=0;
					while(line[tmp_int]!=' ') tmp_int++;
					if(line[tmp_int+2]=='w'){	// find a writable mapping
						// skip the next 5 lines
						for(tmp_int=0; tmp_int<6; tmp_int++)
							fgets(line, 256, mfp);
						// add up private_dirty and private clean
						sscanf(line, "Private_Clean: %d kB", &tmp_int);
						//if(sampleN==0) fprintf(stderr, "%sread=%d\n", line, tmp_int);
						tmp_double+=tmp_int;
						fgets(line, 256, mfp);
						sscanf(line, "Private_Dirty: %d kB", &tmp_int);
						//if(sampleN==0) fprintf(stderr, "%sread=%d\n", line, tmp_int);
						tmp_double+=tmp_int;
						// skip the rest 8 lines
						for(tmp_int=0; tmp_int<8; tmp_int++)
							fgets(line, 256, mfp);
					}
					else if(line[tmp_int+2]=='-'){	//	find a non-writable mapping, skip the next 15 lines
						for(tmp_int=0; tmp_int<15; tmp_int++)
							fgets(line, 256, mfp);
					}
				}
				if(mfp) fclose(mfp);
				if(tmp_double>memory_one_case) memory_one_case=tmp_double;

				//g_snprintf(line, 512, "cp %s curr/smaps%d", filename, sampleN);
				//usleep(1000);
				sampleN++;
				if(sampleN>20000*TIME_OUT_SEC){
					kill(pid, SIGINT);
					break;
				}
			}
			//if(profiler_debug) fprintf(stderr, "%d samples for case %d. memory= %lf\n", sampleN, j, memory_one_case);
			g_snprintf(line, 128, "%lf\n", memory_one_case);
			write(redirectPipe[PIPE_OUT], line, strlen(line));
			//close(redirectPipe[PIPE_OUT]);
			g_free(line);
			g_free(append);
			g_free(filename);
			exit(0);
		}

		// parent process, wait for pid
		wait4(pid, &tmp_int, 0, &usage);
		tmp_int=read(redirectPipe[PIPE_IN], line, 512);
		if(tmp_int<=0) fprintf(stderr, "read nothing from the pipe(%d). testcase %d.\n", tmp_int, i);
		line[tmp_int]='\0';
		sscanf(line, "%lf", &memory_one_case);
		//g_spawn_close_pid(pid);
		memory_t+=memory_one_case;
		time_t+=usage.ru_utime.tv_sec+usage.ru_stime.tv_sec+(usage.ru_utime.tv_usec+usage.ru_stime.tv_usec)/(double)1000000;
		while(wait(&tmp_int)>0);
	}
	//freopen("/dev/tty", "w", stdout);
	close(redirectPipe[PIPE_OUT]);
	close(redirectPipe[PIPE_IN]);
	g_free(line);
	// cmp the output and summarise the correctness
	correctness_t=0;
	if(profile_times>0){
		for(i=0; i<length; i++){
			g_snprintf(filename, 128, "%sout%d", CURRDIR, i);
			line=g_list_nth_data(testcaseResults, i);
			//correctness+=cmpOutput(filename, line);
		}
	}
	g_free(filename);
	g_free(append);
	if(profiler_debug)
		g_printf("memory: %lf\ntime: %lf\n", memory_t, time_t);
	*time=time_t;
	*memory=memory_t;
	*correctness=correctness_t;
	profile_times++;
	return 0;
}
