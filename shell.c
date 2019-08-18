#include<stdio.h>   // printf, perror
#include<unistd.h>  // getcwd
#include<stdlib.h>  // malloc
#include<string.h>  // strcat, strcpy
#include "shell.h"  // display_prompt


char * HOME_DIR;

void set_home_dir() {

    // get directory from which shell is invoked
    char * pwd = (char *) malloc(1024 * sizeof(char));
    if(pwd == NULL) { perror(""); exit(1); }

    if(getcwd(&pwd[1], 1024) == NULL) {   
        perror(""); 

        pwd = "/\0";  // default
    }

    HOME_DIR = pwd;
}

int main() {  // to do : check for possible memory leaks with valgrind
    
    set_home_dir();

    display_prompt();

    interprete_commands();

}