#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main() {
    if(chdir("..") < 0 )
        perror("");
}