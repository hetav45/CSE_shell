#include <stdio.h> // printf
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/sysmacros.h>

void exec_ls(char **arg)
{

    DIR *dir_id;
    struct dirent *dir_stat;
    struct stat file_stat;
    struct passwd *own;
    struct group *grp;

    // parsing
    int flag_l = 0, flag_a = 0;
    char dir_path[1024], time[64], file_path[1024];

    dir_path[0] = '.';
    dir_path[1] = '\0'; // default

    if (arg[1] != NULL)
    {
        for (int i=1; arg[i] != NULL; i++)
        {
            if (strcmp(arg[i], "-l") == 0)
                flag_l = 1;
            else if (strcmp(arg[i], "-a") == 0)
                flag_a = 1;
            else if (strcmp(arg[i], "-la") == 0)
            {
                flag_l = 1;
                flag_a = 1;
            }
            else if (strcmp(arg[i], "-al") == 0)
            {
                flag_l = 1;
                flag_a = 1;
            }
            else
                strcpy(dir_path, arg[i]);
        }
    }

    // check if need to add '/' after file_path
    int len = strlen(dir_path);
    if (len > 0)
    {
        if (dir_path[len - 1] != '/')
        {
            dir_path[len] = '/';
            dir_path[len + 1] = '\0';
        }
    }

    // using readdir and stat to execute ls
    if ((dir_id = opendir(dir_path)) == NULL)
    {
        perror("");
        return;
    }

    // to do : to print total no of files

    while ((dir_stat = readdir(dir_id)) != NULL)
    {

        strcpy(file_path, dir_path);
        strcat(file_path, dir_stat->d_name);

        if (stat(file_path, &file_stat) < 0)
            continue;

        if (flag_l)
        { // man 7 inode

            if ((dir_stat->d_name[0] == '.' && flag_a) || dir_stat->d_name[0] != '.')
            {

                // type of file : not working , gives directory for every other file type than regular file
                switch (file_stat.st_mode & S_IFMT)
                {
                case S_IFDIR:
                    printf("d");
                    break; // dir
                case S_IFCHR:
                    printf("c");
                    break; // character device
                case S_IFBLK:
                    printf("b");
                    break; // block device
                case S_IFIFO:
                    printf("p");
                    break; // pipe
                case S_IFLNK:
                    printf("l");
                    break; // sym link
                case S_IFSOCK:
                    printf("s");
                    break; // socket

                default:
                    printf("-");
                    break; // file
                }

                // permissions
                file_stat.st_mode &S_IRUSR ? printf("r") : printf("-");
                file_stat.st_mode &S_IWUSR ? printf("w") : printf("-");
                file_stat.st_mode &S_IXUSR ? printf("x") : printf("-");
                file_stat.st_mode &S_IRGRP ? printf("r") : printf("-");
                file_stat.st_mode &S_IWGRP ? printf("w") : printf("-");
                file_stat.st_mode &S_IXGRP ? printf("x") : printf("-");
                file_stat.st_mode &S_IROTH ? printf("r") : printf("-");
                file_stat.st_mode &S_IWOTH ? printf("w") : printf("-");
                file_stat.st_mode &S_IXOTH ? printf("x") : printf("-");
                printf(" ");

                // number of hard links
                printf("%3ld ", file_stat.st_nlink);

                // owner
                if ((own = getpwuid(file_stat.st_uid)) == NULL)
                    printf("  owner "); // default
                else
                    printf("%7s ", own->pw_name);

                // group
                if ((grp = getgrgid(file_stat.st_gid)) == NULL)
                    printf("  group "); // default
                else
                    printf("%7s ", grp->gr_name);

                // size
                printf("%9lu  ", file_stat.st_size);

                // last modified
                strcpy(time, ctime(&file_stat.st_mtime));
                if (strlen(time) > 1)
                    time[strlen(time) - 1] = '\0';
                printf("%s  ", time);

                // name
                printf("%s\n", dir_stat->d_name);
            }
        }

        else
        {
            if ((dir_stat->d_name[0] == '.' && flag_a) || dir_stat->d_name[0] != '.')
                printf("%s  ", dir_stat->d_name);
        }
    }

    if (!flag_l)
        printf("\n");
    //if(dir_stat == NULL) perror("");

    closedir(dir_id);
}