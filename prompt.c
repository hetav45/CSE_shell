#include "shell.h"
#include<stdio.h>   // printf, scanf
#include<unistd.h>  // getlogin, getcwd
#include<stdlib.h>  // malloc
#include<string.h>  // strcat
 

int check_rel_path(char * ab_path) {  // checks if relative path is possible, i.e pwd is inside home_dir
    
    int i, flag = 0, len_h = strlen(HOME_DIR), len_a = strlen(ab_path);
    
    // check if HOME_DIR is contained in ab_path
    for(i=0; i<len_h && i<len_a; i++) {
        if(HOME_DIR[i] != ab_path[i]) 
            flag = 1;
    }    

    return flag;  
}

void display_prompt() {  // default values will be displayed upon error
  
    // get user name
    char * u_name = (char *) malloc(128 * sizeof(char));
    if(u_name == NULL) { perror(""); exit(1); }

    if(getlogin_r(u_name, 128)) {  
        perror(""); 
        
        u_name = "user_name\0";  // default
    }

    // get host name
    char * h_name = (char *) malloc(128 * sizeof(char)); 
    if(h_name == NULL) { perror(""); exit(1); }
    
    h_name[0] = '@';

    if(gethostname(&h_name[1], 127) < 0) {  // 1 byte reserved for '@'
        perror(""); 
        
        h_name = "@host_name\0"; // default    
    }

    // get pwd
    char * pwd = (char *) malloc(1024 * sizeof(char));
    if(pwd == NULL) { perror(""); exit(1); }

    if(getcwd(&pwd[1], 1023) == NULL) {  // 1 byte reserved for ':' 
        perror(""); 
    
        pwd = "~\0"; // default   
    }

    // get relative path of pwd
    char * rel_pwd = (char *) malloc(1024 * sizeof(char));
    if(rel_pwd == NULL) { perror(""); exit(1); }

    rel_pwd[0] = ':'; 

    if(check_rel_path(pwd)) {
        strcat(&rel_pwd[1], pwd);
    } 
    else {
        rel_pwd[2] = '~';
        strcat(&rel_pwd[2], &pwd[strlen(HOME_DIR)]);
    }

    // print and free allocated memory
    printf("%s\n", strcat(strcat(u_name, h_name), rel_pwd));

    free(u_name);
    free(h_name);
    free(pwd);
    free(rel_pwd);
}