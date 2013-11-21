#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int  i = 0;
    int * p = 0;
    int m = 0;
    for(i = 0 ; i < 100000; i ++)
    {
        m  = rand() % 10000;  
        p = malloc(sizeof(int) * m);
	memset(p, 0, m); 
        free (p);
    }
    printf("exit normally.\n");
    return 0;

}
