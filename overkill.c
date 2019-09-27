#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void exec_overkill() 
{
    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        killpg(getpgid(i->pid), 9);
    }
}