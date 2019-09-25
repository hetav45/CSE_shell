#include <stdio.h>
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void exec_cronjob(char **argv)
{
    int pid = fork();

    if (pid == 0)
    {
        int i, t = -1, p = -1, start = -1, end = -1;
        char com[64];
        com[0] = '\0';

        for (i = 1; argv[i] != NULL; i++)
        {
            if (strcmp("-c", argv[i]) == 0)
            {
                start = end = i + 1;
                if (argv[i + 1] != NULL)
                    strcpy(com, argv[i + 1]);
                while (argv[i + 1] != NULL && strcmp("-t", argv[i+1]) && strcmp("-p", argv[i+1]))
                {
                    i++;
                    end++;
                }
            }
            else if (strcmp("-t", argv[i]) == 0)
            {
                if (argv[i + 1] != NULL)
                    t = atoi(argv[i + 1]);
            }
            else if (strcmp("-p", argv[i]) == 0)
            {
                if (argv[i + 1] != NULL)
                    p = atoi(argv[i + 1]);
            }
        }
        if (i < 7 || t < 0 || p < 0 || com[0] == '\0')
        {
            printf("%d %d %d\n", i, t, p);
            fprintf(stderr, "invalid arguments or invalid number of arguments\nusage: cronjob -c [command] -t [time peroid >= 0] -p [total time >= 0]\n");
            exit(1);
        }

        argv[end] = NULL;
        for (int j = 0; j <= p; j += t)
        {
            execute_commands(&argv[start]);
            sleep(t);
        }

        exit(0);
    }
}