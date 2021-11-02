#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int dir(char *path, char *format)
{
    int total = 0;

    DIR *d = opendir(path);
    struct dirent *entry = readdir(d);

    while (entry)
    {
        char newPath[strlen(path) + strlen(entry->d_name) + 1];
        strcpy(newPath, path);
        strcat(newPath, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            strcat(newPath, "/");

            char newFormat[strlen(format) + 1];
            strcpy(newFormat, format);
            strcat(newFormat, "\t");

            total += dir(newPath, newFormat);
        }
        else
        {
            struct stat *s;
            stat(newPath, s);

            int size = s->st_size;
            total += size;

            printf("File: %s\tSize:%u\n", entry->d_name, size);
        }

        entry = readdir(d);
    }

    printf("Total size:%u\n", total);
    closedir(d);
}

int main()
{
    dir("./", "");
}