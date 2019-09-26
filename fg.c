#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

void exec_fg(int jobid)
{
    int pid = -1;

    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        if (j == jobid)
            pid = i->pid;
    }

    if (pid == -1)
    {
        fprintf(stderr, "no such background process with job number : %d\n", jobid);
        return;
    }

    tcsetpgrp(STDIN_FILENO, pid);
    exec_kjob(jobid, 18);

    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        if (i->pid == pid)
        {
            fg.pid = pid;
            strcpy(fg.name, i->name);
            fg.status = 1;

            delete_node(i);
        }
    }

    int status;
    waitpid(pid, &status, WUNTRACED);

    if (WIFSTOPPED(status)) 
    {
        if(fg.pid != -1)
            insert_node(fg.pid, fg.name, 0);
        fg.pid = -1;
    }

    tcsetpgrp(STDIN_FILENO, getpid());
}