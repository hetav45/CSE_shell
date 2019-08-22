#include<stdio.h>   // printf, perror
#include<unistd.h>  // getcwd
#include<stdlib.h>  // malloc
#include<string.h>  // strcat, strcpy
#include "shell.h"  // display_prompt


char HOME_DIR[1024];  

void set_home_dir() {

    // path to the executable : readlink to read the symbolic link /proc/pid/exe
    char path[1024] = "/proc/\0", temp[64];
    
    snprintf(temp, 63, "%d", getpid());
    
    strcat(path, temp);
    strcat(path, "/exe");

    if( (readlink(path, HOME_DIR, 1023)) < 0 ) {
        perror("");
        HOME_DIR[0] = '/'; HOME_DIR[1] = '\0';  //default    
    }
    if(strlen(HOME_DIR) > 5)
        HOME_DIR[strlen(HOME_DIR) - 6] = '\0'; // HARDCODED :: EXECUTABLE NAME IS "/SHELL" SO IT IS REMOVED
}

int main() {  // to do : check for possible memory leaks with valgrind
    
    set_home_dir();

    display_prompt();

    interprete_commands();
}