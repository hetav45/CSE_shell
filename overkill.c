#include "shell.h"
#include <stdio.h>

void exec_overkill() 
{
    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        exec_kjob(j, 9);
    }
}