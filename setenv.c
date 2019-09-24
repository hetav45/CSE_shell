#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exec_setenv(char **argv) 
{
    int no = 0;

    while(argv[no] != NULL) 
        no++;

    if(no == 1 || no > 3)  // there will always be one arg at argv[0] ("setenv")
    {
        fprintf(stderr, "invalid number of arguments\nusage : 'setenv var [value]'\n");    
        return;   
    }

    char val[128] = "\0";
    if(no == 3) 
    {
        strcpy(val, argv[2]);
    }

    if(setenv(argv[1], val, -1) < 0)
    {
        perror("");
    }
}