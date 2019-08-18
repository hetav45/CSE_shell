#include<stdio.h>   // printf, scanf
#include<unistd.h>  // getlogin, getcwd
#include<stdlib.h>  // malloc
#include<string.h>  // strcat
#include "shell.h"


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

}