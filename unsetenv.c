#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

void exec_unsetenv(char **argv) 
{
    int no = 0;

    while(argv[no] != NULL) 
        no++;

    if(no < 2) // there will always be one arg at argv[0] ("unsetenv")
    {
        fprintf(stderr, "invalid number of arguments\nusage : 'unsetenv var1 var2 .. varN'\n");   
        return;    
    }

    for(int i=0; argv[i] != NULL; i++) 
    {
        unsetenv(argv[i]);
    }
}