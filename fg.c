#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

void exec_fg(int pid)
{
    if (pid == -1)
    {
        fprintf(stderr, "no arguments given\n");
        return;
    }

    int status;
    while (waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED) >= 0)
    {
        if (WIFCONTINUED(status) == 0)
        {
            exec_kjob(pid, 18); // send SIGCONT
        }
        else
            break;
    }

    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        if (i->pid == pid)
        {
            delete_node(i);
        }
    }

    fg.pid = getpid();
    strcpy(fg.name, "fg");
    fg.status = 1;

    waitpid(pid, NULL, 0);
}