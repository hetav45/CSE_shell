#include <stdio.h>
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void exec_cronjob(char **argv)
{
    int pid = fork();

    if (pid == 0)
    {
        flag_bg = 1;
        setpgid(0, 0);

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
                while (argv[i + 1] != NULL && strcmp("-t", argv[i + 1]) && strcmp("-p", argv[i + 1]))
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
            fprintf(stderr, "invalid arguments or invalid number of arguments\nusage: cronjob -c [command] -t [time peroid >= 0] -p [total time >= 0]\n");
            exit(1);
        }

        argv[end] = NULL;
        for (int j = 0; j <= p; j += t)
        {
            int pid2 = fork();
            if(pid2 == 0) {
                execute_commands(&argv[start]);
                exit(0);
            }
            else 
                waitpid(pid2, NULL, 0);
            sleep(t);
        }

        exit(0);
    }

    insert_node(pid, "cronjob", 1);
}