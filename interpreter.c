#include<stdio.h> // scanf, printf
#include<string.h>  // strcmp
#include<stdlib.h>  // malloc, fflush
#include "shell.h"  // exec*, display_prompt

void interprete_commands() {

    char * str_com = (char *) malloc(32 * sizeof(char)); 
    char * str_arg = (char *) malloc(1024 * sizeof(char));

    while(1) {
        
        scanf("%s", str_com);

        if(strcmp("pwd", str_com) == 0) {
            exec_pwd();
        }

        else if(strcmp("echo", str_com) == 0) {
            scanf("%[^\n]s", str_arg);  // to do : *c vs s ?
            exec_echo(str_arg);
        }

        else if(strcmp("cd", str_com) == 0) {
            scanf("%s", str_arg);
            exec_cd(str_arg);
        }

        else {
            printf("Command not found\n");
            fflush(stdin);
        }

        display_prompt();
    }
}