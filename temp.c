#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main() {
    int opt, argc = 3;
    char * argv[3];
    char a[] = "-a\0";
    char b[] = "abcd\0";
    argv[0] = a;
    argv[1] = a;
    argv[2] = NULL;

    while((opt = getopt(argc, argv, "a:b")) != -1) {
        //printf("%c\n", opt);
        if(optarg != NULL);
            //printf("%s\n", optarg);
    }

    execlp("ls", "gcc", "-l", "..", NULL);
    printf("%d\n", opt);
        
}