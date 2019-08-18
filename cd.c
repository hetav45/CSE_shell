#include "shell.h"  // HOME_DIR
#include<unistd.h>  // chdir
#include<stdio.h>  // perror


void exec_cd(char * dir_path) {

    // no arguments; go to home_dir
    if(dir_path == NULL) {
        
        if(chdir(HOME_DIR) < 0) {
            perror("");
            return;
        }

        return;
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

    if(chdir(dir_path) ) {
        perror("");
        return;
    }
}