#include<stdio.h> // scanf, printf
#include<string.h>  // strcmp, strlen
#include<stdlib.h>  // malloc, fflush
#include "shell.h"  // exec*, display_prompt

void interprete_commands() {  // to do : check for memory loss with strcok

    long unsigned int in_size = 1024;
    char * str_in = (char *) malloc(in_size * sizeof(char)); 
    char * str_arg, * str_com, * temp1, * temp2; 

    while(1) {

        // do not use scanf !!
        getline(&str_in, &in_size, stdin);
        if(strlen(str_in) > 1)
            str_in[strlen(str_in) - 1] = '\0';  // remove trailing \n

        // in case only enter is pressed
        if(str_in[0] == '\0') {
            display_prompt();
            free(str_in);
            continue;
        }

        // for commands sperated by ';'
        temp1 = str_in;
        while((str_com = strtok_r(temp1, ";", &temp1))) {
            
            temp2 = str_com;
            // to trim whitespaces
            while((str_arg = strtok_r(temp2, " ", &temp2))) {
                
                if(strcmp("pwd", str_arg) == 0) {
                    exec_pwd();
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

                else {
                    printf("command not found : %s\n", str_arg);
                    str_arg = strtok_r(temp2, "\0", &temp2);  // this is just to flush the remaining characters
                }
            }
        }

        display_prompt();     
    }

    free(str_in);   
}