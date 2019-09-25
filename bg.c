#include "shell.h"
#include <stdio.h>

void exec_bg(int jobid)
{
    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        if (jobid == j)
        {
            exec_kjob(jobid, 18);
            i->status = 1;
            return;
        }
    }

    fprintf(stderr, "No such process with job number : %d\n", jobid);
}