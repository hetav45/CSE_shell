#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

void exec_jobs(struct child_list *root)
{
    int status;

    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        printf("[%d] ", j);
        j++;

        int ret =  waitpid(i->pid, &status, WNOHANG | WUNTRACED | WCONTINUED); // WHOHANG is non-blocking
        
        if(ret == -1)
        {
            perror("");
            return;
        }
        else if (ret > 0)
        {
            if (WIFSTOPPED(status))
            {
                i->status = 0;
            }

            else
            {
                i->status = 1;
            }
        }

        printf(i->status ? "Running" : "Stopped");
        
        printf(" %s [%d]\n", i->name, i->pid);
    }
}