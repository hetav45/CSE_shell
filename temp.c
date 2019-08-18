#include<stdio.h>
#include<unistd.h>

int main() {
    char *str;
    str = getlogin();
    printf("%s\n", str);
}