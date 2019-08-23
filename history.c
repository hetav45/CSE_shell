#include <stdio.h>
#include <string.h>
#include "shell.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define r_sz 21 // actual size will be 21 - 1 = 20

int start_ptr = 0, end_ptr = 0;

char record[r_sz][1024];

int max(int a, int b)
{
    return a > b ? a : b;
}

void exec_history(char *arg)
{

    int num = 10; // default
    if (arg != NULL)
        num = atoi(arg);

    if (num > 10 || num < 1)
    {
        printf("history: argument should be a number between (including) 1 and 10\n");
        return;
    }

    // circular list
    for (int i = (max(start_ptr, start_ptr + (end_ptr - start_ptr + r_sz) % r_sz - num)) % r_sz; i % r_sz != end_ptr; i++)
    {
        printf("%s\n", record[i % r_sz]);
    }
}

void insert_history(char *arg)
{

    // same commands are not inserted again
    if (strcmp(record[(end_ptr - 1) % r_sz], arg) == 0)
        return;

    strncpy(record[end_ptr], arg, 1023);

    end_ptr = (end_ptr + 1) % r_sz;

    if (end_ptr == start_ptr)
        start_ptr = (start_ptr + 1) % r_sz;
}

void save_history()
{

    char f_path[1024];
    strncpy(f_path, HOME_DIR, 1000); // more characters to come : "/.history"
    strcat(f_path, "/.history");

    int fd;
    if ((fd = open(f_path, O_WRONLY | O_CREAT | O_TRUNC, 00600)) < 0)
    {
        perror("");
        return;
    }

    char buf[r_sz * 1024 + r_sz] = "\0"; // extra r_sz for \n

    for (int i = start_ptr; i % r_sz != end_ptr; i++)
    {
        // for the file we will save the commands in the correct order i.e start_ptr = 0 for the file
        strcat(buf, record[i % r_sz]);
        strcat(buf, "\n");
    }

    if (write(fd, buf, r_sz * 1024 + r_sz) < 0)
        perror("");

    close(fd);
}

void retrieve_history()
{

    char f_path[1024];
    strncpy(f_path, HOME_DIR, 1000); // more characters to come : "/.history"
    strcat(f_path, "/.history");

    int fd;
    if ((fd = open(f_path, O_CREAT | O_RDONLY, 00600)) < 0)
    {
        perror("");
        printf("asfasf\n");
        return;
    }

    char buf[r_sz * 1024 + r_sz]; // extra r_sz for \n
    for (int i = 0; i < (r_sz * 1025); i++)
        buf[i] = 0; // read wont null terminate

    if ((read(fd, buf, r_sz * 1025)) < 0)
    {
        perror("");
        return;
    }

    // now to copy existing values to record
    char *temp1, *temp2;

    temp1 = buf;

    start_ptr = 0; // should br true any way

    for (end_ptr = 0; (temp2 = strtok_r(temp1, "\n", &temp1)) != NULL && end_ptr < r_sz; end_ptr++)
    {

        strncpy(record[end_ptr], temp2, 1023);
    }

    close(fd);
}