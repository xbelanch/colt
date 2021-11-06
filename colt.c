#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv[0];

    struct dirent *dp;
    DIR *dir = opendir(".");
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_type == 8) { // is a file?
            if (dp->d_name[0] == '.')
                continue;

            if (strcmp(dp->d_name, "colt") == 0)
                continue;

            fprintf(stdout, "%s\n", dp->d_name);

            // Read and output the content of file
            FILE *fp = fopen(dp->d_name, "rb");
            char chr;
            while ((chr = getc(fp)) != EOF) {
                fprintf(stdout, "%c", chr);
            }

            fclose(fp);
        }

        if (dp->d_type == 4) { // is a directory

            if (strcmp(".", dp->d_name) == 0
                || strcmp("..", dp->d_name) == 0
                || dp->d_name[0] == '.')
                continue;

            fprintf(stdout, "%s\n", dp->d_name);
        }
    }

    closedir(dir);
    return 0;
}