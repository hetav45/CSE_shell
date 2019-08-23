#include<stdio.h>  // printf
#include<string.h>  // strlen


void exec_echo(char * str) {  // to do : handle the case when commas are not closed

    int flag_s_comma = 0, flag_d_comma = 0, flag_b_slash = 0, flag_space = 1; // for trimming space

    for(int i=0; i < strlen(str); i++) {  // to do : what if echo \0advasdf ?

        // characters inside single inverted comma; printed as is
        if(flag_s_comma) {
            if(str[i] == '\'') 
                flag_s_comma = 0;
            else 
                printf("%c", str[i]);
        }

        // characters inside double inverted comma; printed as is
        else if(flag_d_comma) {
            if(str[i] == '"') 
                flag_d_comma = 0;
            else 
                printf("%c", str[i]);
        }

        // character trailing back slash; devoid of special meaning
        else if(flag_b_slash) {
            printf("%c", str[i]);

            flag_b_slash = 0;
        }

        // nothing special
        else {
            if(str[i] == '\'') 
                flag_s_comma = 1;
            else if(str[i] == '"')
                flag_d_comma = 1;
            else if(str[i] == '\\')  // '\\' is '\' : extra backslash is to escape itself !
                flag_b_slash = 1;
            else if(str[i] == ' ' && !flag_space) {
                flag_space = 1;
                printf("%c", str[i]);
            }
            else {
                if(flag_space && str[i] == ' ') 
                    continue;
                flag_space = 0;
                printf("%c", str[i]);
            }
        }
    }

    printf("\n");
}