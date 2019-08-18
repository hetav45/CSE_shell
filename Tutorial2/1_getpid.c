#include<stdio.h>
#include<unistd.h>

int main(){
    int pid;
    pid = getpid();
    printf("My process ID is %d\n", pid);
    for (int i = 0; i < 1000000000; ++i)
    {
        printf("%d\n", pid);
    }
    return 0;
}
