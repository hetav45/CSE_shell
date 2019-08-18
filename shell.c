#include<stdio.h>  // printf, scanf
#include<unistd.h>  // getlogin, getcwd
#include<stdlib.h>  // calloc
#include<sys/utsname.h>  // uname

char *get_user_name() {
    char * ret = getlogin();
    if(ret == NULL) { 
        perror(""); 
        ret = (char *)calloc(10, sizeof(char));  // 1 more byte allocated for termiantion(\0)
        if(ret == NULL) { perror(""); exit(1); }
        ret = "user_name";  // default
    }
    return ret;
}

char *get_system_name() {  // to do : buf being local var, it should be dealloced after function termiantion, what if it is overwritten ?
    char * ret;
    struct utsname buf;
    if(uname(&buf) < 0) {
        perror("");
        ret = (char *)calloc(12, sizeof(char));  // 1 more byte allocated for termiantion(\0)
        if(ret == NULL) { perror(""); exit(1); }
        ret = "system_name"; // default
    }
    ret = buf.sysname;
    return ret;
}

char *get_pwd() {
    char * ret = calloc(1000, sizeof(char));
    if(getcwd(ret, 1000) == NULL) { perror(""); }
    return ret;
}

int main() {  // to do : check for possible memory leaks with valgrind
    char * p = get_user_name();
    char * q = get_pwd();
    if(q!= NULL) { printf("%s\n", q);}
}