#include "shell.h"  // HOME_DIR
#include <stdio.h>  // printf, perror
#include <unistd.h> // getlogin, getcwd
#include <stdlib.h> // malloc
#include <string.h> // strcat

int check_rel_path(char *ab_path)
{ // checks if relative path is possible, i.e pwd is inside home_dir

    int i, flag = 0, len_h = strlen(HOME_DIR), len_a = strlen(ab_path);

    // check if HOME_DIR is contained in ab_path
    for (i = 0; i < len_h && i < len_a; i++)
    {
        if (HOME_DIR[i] != ab_path[i])
            flag = -1;
    }
    if (len_h > len_a)
        flag = -1;

    // if home_dir == pwd
    if (flag == 0 && len_h == len_a)
        flag = 1;

    return flag;
}

void display_prompt()
{ // default values will be displayed upon error

    bg_handler();
    
    // get user name
    char u_name[128] = "user_name\0"; // default
    for (int i = 10; i < 128; i++)
        u_name[i] = 0;

    if (getlogin_r(u_name, 128))
        perror("");

    // get host name
    char h_name[128] = "host_name\0"; // default
    for (int i = 10; i < 128; i++)
        h_name[i] = 0;

    if (gethostname(h_name, 128) < 0)
        perror("");

    // get pwd
    char pwd[1024] = "\0";
    for (int i = 1; i < 1024; i++)
        pwd[i] = 0;

    if (getcwd(pwd, 1024) == NULL)
        perror("");

    // get relative path of pwd
    char rel_pwd[1024];
    for (int i = 0; i < 1024; i++)
        rel_pwd[i] = 0;

    int temp = check_rel_path(pwd);
    if (temp == -1)
    { // if curr_dir not in home_dir; show ab_path
        strcpy(rel_pwd, pwd);
    }
    else if (temp == 0)
    { // if curr_dir in home_dir but not equal to home_dir;  join with telda
        rel_pwd[0] = '~';
        strcpy(&rel_pwd[1], &pwd[strlen(HOME_DIR)]);
    }
    else if (temp == 1)
    { // if curr_dir equlal to home_dir
        rel_pwd[0] = '~';
        rel_pwd[1] = '\0';
    }

    // print and free allocated memory
    printf("<%s@%s:%s> ", u_name, h_name, rel_pwd);
    fflush(stdout);
}