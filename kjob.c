#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void exec_kjob(int pid, int signal)
{
    if (pid == -1 || signal == -1)
    {
        fprintf(stderr, "invalid number of arguments\nusage : 'kjob [job_id] [signal]'\n");
        return;
    }

    kill(pid, signal);
}