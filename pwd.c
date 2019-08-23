#include <unistd.h> // getcwd
#include <stdio.h>  // printf
#include <stdlib.h> // malloc
#include <stdio.h>  // perror

void exec_pwd()
{

    char *pwd = malloc(1024 * sizeof(char));

    if (getcwd(pwd, 1024) == NULL)
    {
        perror("");
        free(pwd);
        return;
    }

    printf("%s\n", pwd);

    free(pwd);
}