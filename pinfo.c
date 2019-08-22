#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


void exec_pinfo(char * pid) {

    char path[1024], path_2[1024], buf[1024], * temp, field[1024];
    for(int i=0; i<1024; i++) {
        buf[i] = 0;  field[i] = 0;
    } // since read, readlink wont null terminate

    strcpy(path, "/proc/");
    strcat(path, pid);

    // pid
    printf("pid -- %s\n", pid);

    // process status and memory
    strcpy(path_2, path);
    strcat(path_2, "/stat");
    
    // open /proc/pid/stat
    int fd = open(path_2, O_RDONLY);
    if(fd == -1) {
        perror("");
        return;
    }

    if( read(fd, buf, 1023) < 0) {
        perror("");
        close(fd);
        return;
    }

    temp = buf;
    // get the 3rd field : status
    for(int i=0; i<3; i++)
        strcpy(field, strtok_r(temp , " ", &temp));

    if(field != NULL) printf("Process Status -- %s\n", field);

    // get the 23rd = 3 + 20 th field : virtual memory
    for(int i=0; i<20; i++)
        strcpy(field, strtok_r(temp , " ", &temp));

    if(buf != NULL) printf("Memory -- %s\n", field);

    close(fd);

    // path to the executable : readlink to read the symbolic link /proc/pid/exe
    strcpy(path_2, path);
    strcat(path_2, "/exe");

    for(int i=0; i<1024; i++) {
        buf[i] = 0;
    } // since readlink wont null terminate

    if( (readlink(path_2, buf, 1023)) < 0 ) {
        perror("");
        return;    
    }

    printf("Exeecutable Path -- %s\n", buf);
}