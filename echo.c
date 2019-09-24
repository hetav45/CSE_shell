#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h>

void exec_echo(char **arg)
{ // to do : handle the case when commas are not closed

    char * str = (char *) calloc(10 * 1024, sizeof(char));
    if(str == NULL) 
    {
        perror("");
        return;
    }
    for(int i=1; arg[i] != NULL; i++) 
    {
        strcat(str, arg[i]);
        strcat(str, " ");
    }

    int flag_s_comma = 0, flag_d_comma = 0, flag_b_slash = 0;

    for (int i = 0; i < strlen(str); i++)
    { // to do : what if echo \0advasdf ?

        // to print env var
        if(str[i] == '$')
        {
            i++;

            // get name of env var
            char var[32] = "\0";

            for(int j= 0; str[i] != ' ' && str[i] != '\t' && str[i] != '\0' && str[i] != '\n'; j++, i++)
            {
                var[j] = str[i];
            }
            // str[i] is not yet traversed
            i--;

            printf("%s", getenv(var));
        }

        // characters inside single inverted comma; printed as is
        else if (flag_s_comma)
        {
            if (str[i] == '\'')
                flag_s_comma = 0;
            else
                printf("%c", str[i]);
        }

        // characters inside double inverted comma; printed as is
        else if (flag_d_comma)
        {
            if (str[i] == '"')
                flag_d_comma = 0;
            else
                printf("%c", str[i]);
        }

        // character trailing back slash; devoid of special meaning
        else if (flag_b_slash)
        {
            printf("%c", str[i]);

            flag_b_slash = 0;
        }

        // nothing special
        else
        {
            if (str[i] == '\'')
                flag_s_comma = 1;
            else if (str[i] == '"')
                flag_d_comma = 1;
            else if (str[i] == '\\') // '\\' is '\' : extra backslash is to escape itself !
                flag_b_slash = 1;
            else
                printf("%c", str[i]);
        }
    }

    free(str);

    printf("\n");
}