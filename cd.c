#include "shell.h"  // HOME_DIR
#include<unistd.h>  // chdir
#include<stdio.h>  // perror
#include<string.h>  // strlen


void exec_cd(char * dir_path) {

    // no arguments; go to home_dir
    if(dir_path == NULL) {
        
        if(chdir(HOME_DIR) < 0) {
            perror("");
            return;
        }

        return;
    }

    // remove quotes : why do they not work with chdir ? 
    if(dir_path[0] == '"' || dir_path[0] == '\'') {
        if(strlen(dir_path) > 0) {
            dir_path[strlen(dir_path) - 1] = '\0';  
            dir_path = &dir_path[1];
        }
    }

    // telda expansion
    if(dir_path[0] == '~') {
        
        // first change the curr_dir to home
        if(chdir(HOME_DIR) < 0) {
            perror("");
            return;
        }
        
        // now replace ~ by . and proceed normally
        dir_path[0] = '.';
    }

    if(chdir(dir_path) < 0) {
        perror("");
        return;
    }
}