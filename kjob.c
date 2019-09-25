#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

void exec_kjob(int jobid, int signal)
{
    int pid = -1;

    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        if(j == jobid)
            pid = i->pid;
    }

    if (pid == -1)
    {
        fprintf(stderr, "no such background process with job number : %d\n", jobid);
        return;
    }

    kill(pid, signal);
}