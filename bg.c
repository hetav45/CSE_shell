#include "shell.h"
#include <stdio.h>

void exec_bg(int pid) 
{
    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        if(i->pid == pid) 
        {
            exec_kjob(pid, 18);
            i->status = 1;
            return;
        }
    }

    fprintf(stderr, "No such process with pid : %d", pid);
}