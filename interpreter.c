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
int flag_bg = 0;

struct child_list *root;
struct child_list fg;

char *init();                                      // initize
void scan(char *str);                              // scans a full line
void interprete_commands(char *str);               // interprete and executes commands with redirection, piping, and bg processes
void dealloc(char **argv, int i);                  // dealloc memory
void restore_io(int flags[], int saved_fd[]);      // restore fd of std_in/out
void insert_node(int pid, char *name, int signal); // insert a node to the processes linked list
void delete_node(struct child_list *i);            // deletes a node form the processes linked list
void bg_handler();                                 // cheks for completed background processes
void execute_commands(char **argv);                // executes commands

void ctrlC_handler()
{
    return;
}

void soft_kill_handler()
{
    return;
}

void ctrlC_core_dumped_handler()
{
    return;
}

void ctrlZ_handler()
{
    return;
    if (fg.pid != -1)
        insert_node(fg.pid, fg.name, 0);
    fg.pid = -1;

    return;
}

void bg_to_fg_handler()
{
    tcsetpgrp(STDIN_FILENO, getpid());
    return;
}

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
    int saved_fd[3], flags[3] = {0, 0, 0}; // 0-std_in, 1-std_out, 2-pipe_read_end

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
            flags[1] = 1;

            // get file name
            str_arg = strtok_r(temp, " \t", &temp);

            int fd_out = -1;
            saved_fd[1] = dup(1);
            if (str_arg == NULL)
            {
                printf("File not specified for output redirection\n");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            if ((fd_out = open(str_arg, O_TRUNC | O_CREAT | O_WRONLY, 00644)) < 0)
            {
                perror("");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            // copy stdout fd to given file
            dup2(fd_out, 1);

            i--; // since no token stored in this iteration
        }
        else if (strcmp(str_arg, ">>") == 0)
        {
            flags[1] = 1;

            // get file name
            str_arg = strtok_r(temp, " \t", &temp);

            int fd_out = -1;
            saved_fd[1] = dup(1);
            if (str_arg == NULL)
            {
                printf("File not specified for output redirection\n");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            if ((fd_out = open(str_arg, O_CREAT | O_APPEND | O_WRONLY, 00644)) < 0) // appeding for double redirection : only difference
            {
                perror("");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            // copy stdout fd to given file
            dup2(fd_out, 1);

            i--; // since no token stored in this iteration
        }
        else if (strcmp(str_arg, "<") == 0)
        {
            flags[0] = 1;

            // get file name
            str_arg = strtok_r(temp, " \t", &temp);

            int fd_in = -1;
            saved_fd[0] = dup(0);
            if (str_arg == NULL)
            {
                printf("File not specified for input redirection\n");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            if ((fd_in = open(str_arg, O_RDONLY)) < 0)
            {
                perror("Input redirection error: ");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            // copy stdin fd to given file
            dup2(fd_in, 0);

            i--; // since no token stored in this iteration
        }
        else if (strcmp(str_arg, "|") == 0)
        {
            flags[1] = flags[2] = 1;

            int pipefd[2];
            if (pipe(pipefd) < 0)
            {
                perror("");
                restore_io(flags, saved_fd); // restore std_in/out if needed
                dealloc(argv, i - 1);
                return;
            }

            // copy stdout fd to pipe fd
            saved_fd[1] = dup(1);
            dup2(pipefd[1], 1);

            // close reading end
            //close(pipefd[0]);

            // execute command before the pipe
            argv[i] = NULL;
            execute_commands(argv);
            restore_io(flags, saved_fd);

            // command after pipe
            flags[0] = flags[2] = 1;

            close(pipefd[1]);     // close writing end of the pipe
            dup2(saved_fd[1], 1); // restore stdout

            saved_fd[0] = dup(0);
            saved_fd[2] = pipefd[0]; // copy read end fd so it can be closed later

            // copy stdin fd to pipe fd
            dup2(pipefd[0], 0);

            // reset argv
            for (i--; i >= 0; i--)
                free(argv[i]);
        }
        else if (strcmp(str_arg, "&") == 0)
        {
            flag_bg = 1;
            argv[i] = NULL;

            int pid = fork();

            if (pid == 0)
            {
                //setpgid(0, 0); // NOT WORKING

                execute_commands(argv);
                exit(0);
            }
            // else : only parent will exec this
            else
            {
                insert_node(pid, argv[0], 1);
                restore_io(flags, saved_fd); // restore std_in/out if needed

                // reset argv
                for (i--; i >= 0; i--)
                    free(argv[i]);
            }

            flag_bg = 0;
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
        execute_commands(argv);
    }

    restore_io(flags, saved_fd); // restore std_in/out if needed
    dealloc(argv, i - 1);
}

char *init()
{
    signal(SIGINT, ctrlC_handler);
    signal(SIGQUIT, ctrlC_core_dumped_handler);  
    signal(SIGTSTP, ctrlZ_handler);
    signal(SIGTTOU, bg_to_fg_handler);
    signal(SIGTERM, soft_kill_handler);

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

    fg.pid = -1; // no process in bakground initialy;
    fg.next = fg.pre = NULL;

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

    char up[4] = {'\033', '[', 'A', '\0'};

    // check for 'up'
    if (strncmp(str, up, 3) == 0)
    {
        int cnt = 1;
        for (; cnt < 10 && strncmp(str + 3 * cnt, up, 3) == 0; cnt++)
            ;

        get_history(cnt, str);
        if (str[0] != '\0')
        {
            bg_handler();
            display_prompt();
            printf("%s\n", str);
        }
    }

    // if only enter is pressed
    if (str[0] == '\0')
    {
        bg_handler();
        display_prompt();
        scan(str);
    }
    else
    {
        insert_history(str);
    }
}

void insert_node(int pid, char *name, int status)
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
    new->pid = pid;
    new->status = status;
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

void dealloc(char **argv, int i)
{
    for (; i >= 0; i--)
    {
        free(argv[i]);
    }
    free(argv);
}

void restore_io(int flags[], int saved_fd[])
{
    for (int i = 0; i < 2; i++)
    {
        if (flags[i])
        {
            dup2(saved_fd[i], i);
            close(saved_fd[i]);
        }

        // reset flags
        flags[i] = 0;
    }

    if (flags[2])
        close(saved_fd[2]);
    flags[2] = 0;
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

void execute_commands(char **argv)
{
    if (argv[0] == NULL)
        return;

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
                if ((temp = (char *)malloc(64 * sizeof(char))) == NULL)
                {
                    perror("");
                    return;
                }
                snprintf(temp, 63, "%d", getppid());
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

    else if (strcmp("setenv", argv[0]) == 0)
    {
        // can not fork
        exec_setenv(argv);
    }

    else if (strcmp("unsetenv", argv[0]) == 0)
    {
        exec_unsetenv(argv);
    }

    else if (strcmp("kjob", argv[0]) == 0)
    {

        int arg1 = -1, arg2 = -1;
        if (argv[1] != NULL)
            arg1 = atoi(argv[1]);
        else
        {
            fprintf(stderr, "invalid number of arguments\nusage : 'kjob [job_id] [signal]'\n");
            return;
        }
        if (argv[2] != NULL)
            arg2 = atoi(argv[2]);
        else
        {
            fprintf(stderr, "invalid number of arguments\nusage : 'kjob [job_id] [signal]'\n");
            return;
        }

        int pid = fork();
        if (pid == 0)
        {
            exec_kjob(arg1, arg2);
            exit(0);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    else if (strcmp("jobs", argv[0]) == 0)
    {
        exec_jobs(root);
    }

    else if (strcmp("overkill", argv[0]) == 0)
    {
        exec_overkill();
    }

    else if (strcmp("fg", argv[0]) == 0)
    {
        int jobid = -1;
        if (argv[1] != NULL)
            jobid = atoi(argv[1]);
        else
        {
            fprintf(stderr, "no arguments given for fg\n");
            return;
        }

        exec_fg(jobid);
    }

    else if (strcmp("bg", argv[0]) == 0)
    {
        int jobid = -1;
        if (argv[1] != NULL)
            jobid = atoi(argv[1]);
        else
        {
            fprintf(stderr, "no arguments given for bg\n");
            return;
        }

        exec_bg(jobid);
    }

    else if (strcmp("cronjob", argv[0]) == 0)
    {
        exec_cronjob(argv);
    }

    else if (strcmp("quit", argv[0]) == 0)
    {
        save_history();
        exit(0);
    }

    else
    {
        if (flag_bg)
        {
            setpgid(0, 0);
            if (execvp(argv[0], argv) < 0)
            {
                if (errno == 2) // for no such file or dir
                    fprintf(stderr, "Command not found : %s\n", argv[0]);
                else
                    perror("");

                exit(1); // bc exec will return if failed

                // no dealloc case anyway the process will terminate
            }
        }
        else
        {
            int pid = fork();

            // exec overwrites the current process, so have to create a child process
            if (pid == 0)
            {
                setpgid(0, 0);
                tcsetpgrp(STDIN_FILENO, getpid());
                if (execvp(argv[0], argv) < 0)
                {
                    if (errno == 2) // for no such file or dir
                        fprintf(stderr, "Command not found : %s\n", argv[0]);
                    else
                        perror("");

                    exit(1); // bc exec will return if failed

                    // no dealloc case anyway the process will terminate
                }
            }
            else
            {
                fg.pid = pid;
                strcpy(fg.name, argv[0]);
                fg.status = 1;

                int status;
                waitpid(pid, &status, WUNTRACED);
                if (WIFSTOPPED(status))
                {
                    if (fg.pid != -1)
                        insert_node(fg.pid, fg.name, 0);
                    fg.pid = -1;
                }

                tcsetpgrp(STDIN_FILENO, getpid());
            }
        }
    }
}
