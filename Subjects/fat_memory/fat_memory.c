#include <stdio.h>
#include <stdlib.h>

#define VAIN 1

typedef struct _list{
	int* chunk;
	struct _list *next;
}list;

int count;

void add(list *head, list *entry){
	list *p=head, *q=p->next;
	while(q && *(q->chunk)<count){
		p->next=q->next;
		free(q->chunk);
		free(q);
		q=p->next;
	}
	while(q && *(q->chunk)<*(entry->chunk)){
		p=q;
		q=q->next;
	}
	p->next=entry;
	entry->next=q;
}

int main(int argc, char **argv){
	if(argc<2){
		printf("Invoke the program with a file name.\n");
		return 0;
	}
	FILE* fp=fopen(argv[1], "r");
	if(!fp){
		printf("open file mallocBase.txt failed.\n");
		exit(0);
	}
	list head;
	head.chunk=NULL;
	head.next=NULL;
	list *p;
	char line[128]={0};
	int size, lifetime;
	count=0;
	int i, j;
	while(!feof(fp)){
		if(fgets(line, 128, fp)==NULL) break;
		count++;
		sscanf(line, "%d %d\n", &size, &lifetime);
		//printf("size=%d, lifetime=%d\n", size, lifetime);
		p=malloc(sizeof(list));
		p->chunk=malloc(size);
		*(p->chunk)=lifetime+count;
		p->next=NULL;
		add(&head, p);
		for(i=0; i<VAIN; i++){
			for(j=1; j<size/32; j++){
				p->chunk[j]=size/32-j;
			}
		}
	}
	p=&head;
	list *q=p->next;
	while(q){
		p=q;
		q=q->next;
		free(p->chunk);
		free(p);
	}
	return 0;
}
