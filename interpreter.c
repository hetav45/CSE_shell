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

long unsigned int in_size = 1024;
int flags[3];  // 0 - ">", 1 - ">>", 2 - "<"

struct child_list
{
    int pid;
    char name[64];
    struct child_list *next;
    struct child_list *pre;
};

struct child_list *root;

char *init();                          // initize
void scan(char *str);                  // scans a full line
void interprete_commands(char *str);   // interprete and executes commands with redirection, piping, and bg processes
void insert_node(int pid, char *name); // insert a node to the processes linked list
void delete_node();                    // deletes a node form the processes linked list
void bg_handler();                     // cheks for completed background processes
void execute_commands(char **argv, int flags[]);    // executes commands

void parse_commands() // scans and seperates semicolons
{

    char *str_in = init();

    while (1)
    {
        scan(str_in);

        // semicolon seprator
        char *temp, *str_com;

        temp = str_in;
        while ((str_com = strtok_r(temp, ";", &temp)))
        {
            interprete_commands(str_com);

            bg_handler();
            display_prompt();
        }
    }
}

void interprete_commands(char *str)
{
    char *temp, *str_arg;
    flags[0] = flags[1] = flags[2] = 0;  // reset flags

    // prepare argv which stores command arguments
    char **argv = (char **)malloc(64 * sizeof(char *));
    if (argv == NULL)
    {
        perror("");
        exit(1);
    }

    temp = str;

    int i; // i keeps track of number of blocks alloced
    for (i = 0; (str_arg = strtok_r(temp, " \t", &temp)); i++)
    {
        // check for special charcaters

        if (strcmp(str_arg, ">") == 0)
        {
            flags[0] = 1;
        }
        else if (strcmp(str_arg, ">>") == 0)
        {
            flags[1] = 1;
        }
        else if (strcmp(str_arg, "<") == 0)
        {
            flags[2] = 1;
        }
        else if (strcmp(str_arg, "|") == 0)
        {
        }
        else if (strcmp(str_arg, "&") == 0)
        {
            argv[i] = NULL;

            int pid = fork();

            if (pid == 0)
            {
                execute_commands(argv, flags);
                exit(0);
            }
            // else : only parent will exec this

            insert_node(pid, argv[0]); // insert into jobs list

            // reset argv
            for (i--; i >= 0; i--)
                free(argv[i]);
        }
        else
        {
            argv[i] = (char *)malloc(256 * sizeof(char));
            if (argv[i] == NULL)
            {
                perror("");
                exit(1);
            }

            strcpy(argv[i], str_arg);
        }
    }

    // normal command
    if (i > 0)
    {
        argv[i] = NULL;
        execute_commands(argv, flags);
    }

    for(i--; i>=0; i--) 
        free(argv[i]);
}

char *init()
{
    display_prompt();

    // get past commands from .history
    retrieve_history();

    // intitilise data structutre for stroring processes
    root = (struct child_list *)calloc(1, sizeof(struct child_list));
    if (root == NULL)
    {
        perror("");
        exit(1);
    }
    root->next = NULL;
    root->pre = NULL;
    root->pid = getpid();
    strcpy(root->name, "./shell");

    // allocate memory for input string
    char *str = (char *)malloc(in_size * sizeof(char));
    if (str == NULL)
    {
        perror("");
        exit(1);
    }

    return str;
}

void scan(char *str)
{
    // do not use scanf !!
    getline(&str, &in_size, stdin);
    if (strlen(str) > 0)
        str[strlen(str) - 1] = '\0'; // remove trailing \n

    // if only enter is pressed
    if (str[0] == '\0')
    {
        display_prompt();
        scan(str);
    }
    else
    {
        insert_history(str);
    }
}

void insert_node(int pid, char *name)
{
    struct child_list *temp = root;

    // temp itself will not be NULL, since process shell.c will be running always
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    struct child_list *new = (struct child_list *)calloc(1, sizeof(struct child_list));
    if (new == NULL)
    {
        perror("");
        exit(1);
    }

    // define the node values
    new->next = NULL;
    new->pre = temp;
    strcpy(new->name, name);
    temp->next = new;
}

void delete_node(struct child_list *i)
{
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

void bg_handler()
{
    int status;

    for (struct child_list *i = root->next; i != NULL; i = i->next)
    {
        if ((waitpid(i->pid, &status, WNOHANG)) > 0) // WHOHANG is non-blocking
        {
            printf("\nProcess %s with pid %d ", i->name, i->pid);
            fflush(stdout);
            if (WIFSIGNALED(status))
                psignal(status, "");
            else
                printf("exited normally\n");

            // delete node
            delete_node(i);
        }
    }
}

void execute_commands(char **argv, int flags[])
{

    if (strcmp("pwd", argv[0]) == 0)
    {
        exec_pwd();
    }

    else if (strcmp("echo", argv[0]) == 0)
    {
        exec_echo(argv);
    }

    else if (strcmp("cd", argv[0]) == 0)
    {
        exec_cd(argv[1]);
    }

    else if (strcmp("ls", argv[0]) == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            exec_ls(argv);
            exit(0);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    else if (strcmp("history", argv[0]) == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            exec_history(argv[1]);
            exit(0);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    else if (strcmp("pinfo", argv[0]) == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            char *temp;
            if (argv[1] == NULL)
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
                exec_pinfo(argv[1]);
            exit(0);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    else if (strcmp("quit", argv[0]) == 0)
    {
        save_history();
        exit(0);
    }

    else
    {
        int pid = fork();

        // exec overwrites the current process, so have to create a child process
        if (pid == 0)
        {
            if (execvp(argv[0], argv) < 0)
            {
                if (errno == 2) // for no such file or dir
                    printf("Command not found : %s\n", argv[0]);
                else
                    perror("");

                exit(1); // bc exec will return if failed

                // no dealloc case anyway the process will terminate
            }
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }
}
