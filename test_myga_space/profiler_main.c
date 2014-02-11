#include "profiler.h"

gint profiler_debug;

void readTestcases(){
	FILE* fp=fopen("testcases.txt", "r+");
	if(!fp){
		g_printf("Read Testcases fail.\n");
		return;
	}
	gchar* line;
	gint i;
	while(!feof(fp)){
		line=g_malloc0(128*sizeof(gchar));
		if(!fgets(line, 128, fp)){
			continue;
		}
		i=0;
		while(line[i]!='\n' && line[i]!='\0') i++;
		if(i==0) continue;
		if(line[i]=='\n') line[i]='\0';
		testcases=g_list_append(testcases, line);
	}
	fullTest=g_list_length(testcases)<=CASES_PER_PROG? 1:0;
	fclose(fp);
}

void main(){
	profiler_debug=1;
	logfp=fopen("log.txt", "w+");
	randomUtility=g_rand_new();
	readTestcases();
	double time, memory, correctness;
	profile(&time, &memory, &correctness, 0);
	g_rand_free(randomUtility);
	g_list_free(testcases);
	fclose(logfp);
}
