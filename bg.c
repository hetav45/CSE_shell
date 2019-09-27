#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void exec_bg(int jobid)
{
    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        if (jobid == j)
        {
            killpg(getpgid(i->pid), 18);
            i->status = 1;
            return;
        }
    }

    fprintf(stderr, "No such process with job number : %d\n", jobid);
}