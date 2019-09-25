#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

void exec_jobs(struct child_list *root)
{
    int status;

    int j = 1;
    for (struct child_list *i = root->next; i != NULL; i = i->next, j++)
    {
        int ret = waitpid(i->pid, &status, WNOHANG | WUNTRACED | WCONTINUED); // WHOHANG is non-blocking

        if (ret == -1)
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
            else if (WIFSIGNALED(status))
            {
                printf("\nProcess %s with pid %d ", i->name, i->pid);
                fflush(stdout);
                psignal(status, "");
                // delete node
                delete_node(i);
                continue;
            }
            else if (WIFEXITED(status))
            {
                printf("\nProcess %s with pid %d ", i->name, i->pid);
                printf("exited normally\n");
                // delete node
                delete_node(i);
                continue;
            }
            else
            {
                i->status = 1;
            }
        }
        printf("[%d] ", j);

        printf(i->status ? "Running" : "Stopped");

        printf(" %s [%d]\n", i->name, i->pid);
    }
}