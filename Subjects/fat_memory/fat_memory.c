#include <stdlib.h>


int main()
{
    int  i = 0;
    int * p = 0;
    int m = 0;
    for(i = 0 ; i < 100000; i ++)
    {
        m  = rand() % 10000;  
        p = malloc(sizeof(int) * m); 
        free (p);
    }
    return 0;

}
