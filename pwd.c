#include<unistd.h>

char *get_pwd() {
    char * ret = calloc(1000, sizeof(char));
    if(getcwd(ret, 1000) == NULL) { perror(""); }
    return ret;
}