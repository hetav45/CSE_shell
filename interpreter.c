#include<stdio.h> // scanf, printf
#include<string.h>  // strcmp, strlen
#include<stdlib.h>  // malloc, fflush
#include "shell.h"  // exec*, display_prompt
#include<unistd.h>  // execvp
#include<sys/wait.h>  // wait
#include<errno.h>


void interprete_commands() {  // to do : check for memory loss with strcok

    long unsigned int in_size = 1024;
    char * str_in;
    if((str_in = (char *) malloc(in_size * sizeof(char))) < 0) {
        perror("");
        return;
    } 
    char * str_arg, * str_com, * temp1, * temp2, * temp3; 

    retrieve_history();

    while(1) {

        // do not use scanf !!
        getline(&str_in, &in_size, stdin);
        if(strlen(str_in) > 0)
            str_in[strlen(str_in) - 1] = '\0';  // remove trailing \n

        // in case only enter is pressed
        if(str_in[0] == '\0') {
            display_prompt();
            free(str_in);
            continue;
        }

        insert_history(str_in);

        // for commands sperated by ';'
        temp1 = str_in;
        while((str_com = strtok_r(temp1, ";", &temp1))) {
            
            temp2 = str_com;
            // to trim whitespaces
            while((str_arg = strtok_r(temp2, " \t", &temp2))) {
                
                if(strcmp("pwd", str_arg) == 0) {
                    exec_pwd();
                    // removing junk
                    str_arg = strtok_r(temp2, "\0", &temp2);
                }

                else if(strcmp("echo", str_arg) == 0) {
                    // get the remaining string before ';'
                    str_arg = strtok_r(temp2, "\0", &temp2);
                    exec_echo(str_arg);
                }

                else if(strcmp("cd", str_arg) == 0) {
                    // get the remaining string before ';'
                    str_arg = strtok_r(temp2, "\0", &temp2);
                    exec_cd(str_arg);
                }

                else if(strcmp("ls", str_arg) == 0) {
                    // get the remaining string before ';'
                    str_arg = strtok_r(temp2, "\0", &temp2);
                    exec_ls(str_arg);
                }

                else if(strcmp("history", str_arg) == 0) {
                    // get the remaining string before ';'
                    str_arg = strtok_r(temp2, "\0", &temp2);
                    exec_history(str_arg);
                }

                else if(strcmp("pinfo", str_arg) == 0) {
                    // get the remaining string before ';'
                    str_arg = strtok_r(temp2, "\0", &temp2);
                    if(str_arg == NULL) {
                        if( (temp3 = (char *) malloc(64 * sizeof(char))) < 0) {
                            perror("");
                            free(str_in);
                            return;
                        }
                        snprintf(temp3, 63, "%d", getpid());
                        exec_pinfo(temp3);
                        free(temp3);    
                    }
                    else 
                        exec_pinfo(str_arg);
                }

                else if(strcmp("exit", str_arg) == 0) {
                    free(str_in);
                    save_history();
                    return;
                }

                else {

                    int pid = fork();

                    // exec overwrites the current process, so have to create a child process
                    if(pid == 0) {                

                        // prepare argv
                        char **argv = (char **) malloc(32 * sizeof(char *)); 

                        if(argv == NULL) {
                            perror("");
                            exit(1);
                        }

                        argv[0] = (char *) malloc(256 * sizeof(char));
                        if(argv[0] == NULL) {
                            perror("");
                               
                            exit(1);
                        }

                        strcpy(argv[0], str_arg);

                        int j;  // keeps track of number of blocks alloced
                        for(j = 1; (str_arg = strtok_r(temp2, " \t", &temp2)); j++) {
                            argv[j] = (char *) malloc(256 * sizeof(char));
                            if(argv[j] == NULL) {
                                perror("");
                                
                                exit(1);
                            }   
                            strcpy(argv[j], str_arg);
                        }
                        argv[j] = NULL;
                        
                        if(execvp(argv[0], argv) < 0) {
                   
                            if(errno == 2)  // for no such file or dir
                                printf("Command not found : %s\n", argv[0]);
                            else 
                                perror("");
                            
                            free(str_in);

                            for(int i = 0; i < j; i++)  // j is the number of blocks alloced
                                free(argv[i]);
                            free(argv);

                            exit(1);  // bc exec will return if failed
                        }
                    }
                    else {
                        // the remaining string is already used up in the child
                        strtok_r(temp2, "\0", &temp2);
                        
                        wait(NULL);
                    }
                }
            }
        }

        display_prompt();     
    }

    free(str_in);   
}