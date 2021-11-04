#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

int dir(char *path, char *format)
{
    int total = 0;

    DIR *d = opendir(path);

    if (d == NULL)
    {
        printf("Exit due to error: %s", strerror(errno));
        return -1;
    }

    struct dirent *entry = readdir(d);

    while (entry)
    {
        char newPath[strlen(path) + strlen(entry->d_name) + 1];
        strcpy(newPath, path);
        strcat(newPath, entry->d_name);

        if (entry->d_type == DT_DIR)
        {

            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".git") != 0)
            {

                strcat(newPath, "/");

                char newFormat[strlen(format) + 1];
                strcpy(newFormat, format);
                strcat(newFormat, "\t");
                printf("%sFolder: %s\n", format, entry->d_name);
                total += dir(newPath, newFormat);
            }
        }
        else
        {
            struct stat s;
            stat(newPath, &s);

            int size = s.st_size;
            total += size;

            printf("%sFile: %-20sSize:%u\n", format, entry->d_name, size);
        }

        entry = readdir(d);
    }

    printf("\n%sTotal size:%u\n", format, total);
    closedir(d);

    return total;
}

int main(int argc, char *argv[])
{
    char path[100];

    if (argc > 1)
    {
        strcpy(path, argv[1]);
    }
    else
    {
        fgets(path, sizeof(path) - 1, stdin);
    }

    dir(path, "");
    return 0;
}
