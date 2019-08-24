#include <stdio.h>    // scanf, printf
#include <string.h>   // strcmp, strlen
#include <stdlib.h>   // malloc, fflush
#include "shell.h"    // exec*, display_prompt
#include <unistd.h>   // execvp
#include <sys/wait.h> // wait
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

int pid_g = -1, flag_bg = 0;

struct child_list
{
    int pid;
    char name[64];
    struct child_list *next;
    struct child_list *pre;
};

struct child_list *root;

void bg_handler();
void execute_commands(char **arg);

void interprete_commands()
{

    long unsigned int in_size = 1024;
    char *str_in;
    if ((str_in = (char *)malloc(in_size * sizeof(char))) < 0)
    {
        perror("");
        return;
    }
    char *str_arg, *str_com, *temp1, *temp2;

    retrieve_history();

    root = (struct child_list *)calloc(1, sizeof(struct child_list));
    root->next = NULL;
    root->pre = NULL;
    root->pid = getpid();
    strcpy(root->name, "./shell");

    display_prompt();

    while (1)
    {

        // do not use scanf !!
        getline(&str_in, &in_size, stdin);
        if (strlen(str_in) > 0)
            str_in[strlen(str_in) - 1] = '\0'; // remove trailing \n

        // in case only enter is pressed
        if (str_in[0] == '\0')
        {
            display_prompt();
            continue;
        }

        insert_history(str_in);

        // for commands sperated by ';'
        temp1 = str_in;
        while ((str_com = strtok_r(temp1, ";", &temp1)))
        {

            flag_bg = 0;

            // prepare argv
            char **argv = (char **)malloc(32 * sizeof(char *));
            if (argv == NULL)
            {
                perror("");
                free(str_in);
                exit(1);
            }

            temp2 = str_com;

            int j; // keeps track of number of blocks alloced
            for (j = 0; (str_arg = strtok_r(temp2, " \t", &temp2)); j++)
            {
                argv[j] = (char *)malloc(256 * sizeof(char));
                if (argv[j] == NULL)
                {
                    perror("");
                    free(str_in);
                    exit(1);
                }
                if (strcmp(str_arg, "&") == 0)
                {
                    flag_bg = 1;
                    pid_g = fork();
                    if (pid_g != 0)
                    {
                        struct child_list * node = (struct child_list *)calloc(1, sizeof(struct child_list));
                        if( node == NULL) 
                        {
                            perror("");
                            exit(1);
                        }
                        node->next = root->next;
                        if(root->next != NULL)
                            root->next->pre = node;
                        root->next = node;
                        node->pre = root;

                        node->pid = pid_g;
                        strcpy(node->name, argv[0]);
                    }
                    break;
                }
                strcpy(argv[j], str_arg);
            }
            argv[j] = NULL; // null terminated 2d arr

            if ((flag_bg && pid_g == 0) || !flag_bg)
                execute_commands(argv);

            for (int i = 0; i < j; i++) // j is the number of blocks alloced
                free(argv[i]);
            free(argv);

            if (flag_bg && pid_g == 0)
                exit(0);
        }

        display_prompt();
    }

    free(str_in);
}

void execute_commands(char **arg)
{

    if (strcmp("pwd", arg[0]) == 0)
    {
        exec_pwd();
    }

    else if (strcmp("echo", arg[0]) == 0)
    {
        exec_echo(arg);
    }

    else if (strcmp("cd", arg[0]) == 0)
    {
        exec_cd(arg[1]);
    }

    else if (strcmp("ls", arg[0]) == 0)
    {
        exec_ls(arg);
    }

    else if (strcmp("history", arg[0]) == 0)
    {
        exec_history(arg[1]);
    }

    else if (strcmp("pinfo", arg[0]) == 0)
    {
        char *temp;
        if (arg[1] == NULL)
        {
            if ((temp = (char *)malloc(64 * sizeof(char))) < 0)
            {
                perror("");
                return;
            }
            snprintf(temp, 63, "%d", getpid());
            exec_pinfo(temp);
            free(temp);
        }
        else
            exec_pinfo(arg[1]);
    }

    else if (strcmp("exit", arg[0]) == 0)
    {
        save_history();
        flag_exit = 1;
        exit(0);
    }

    else
    {
        if (!flag_bg)
            pid_g = fork();

        // exec overwrites the current process, so have to create a child process
        if (pid_g == 0)
        {
            if (execvp(arg[0], arg) < 0)
            {
                if (errno == 2) // for no such file or dir
                    printf("Command not found : %s\n", arg[0]);
                else
                    perror("");

                exit(1); // bc exec will return if failed

                // no dealloc case anyway the process will terminate
            }
        }
        else
        {
            wait(NULL);
        }
    }
}

void bg_handler()
{
    int status;

    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        if ((waitpid(i->pid, &status, WNOHANG)) > 0) // WHOHANG is non-blocking
        {
            printf("\nProcess %s with pid %d ", i->name, i->pid);
            fflush(stdout);
            if(WIFSIGNALED(status)) 
                psignal(status, "");
            else 
                printf("exited normally\n");

            // delete node
            if (i->pre == NULL)
            {
                root = i->next;
                i->next->pre = NULL;
                free(i);
            }
            else if (i->next == NULL)
            {
                i->pre->next = NULL;
                free(i);
            }
            else
            {
                i->pre->next = i->next;
                i->next->pre = i->pre;
                free(i);
            }
        }
    }
}