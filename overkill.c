#include "shell.h"
#include <stdio.h>

void exec_overkill() 
{
    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        exec_kjob(i->pid, 9);
    }
}