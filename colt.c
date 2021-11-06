#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return NULL;
    return dot + 1;
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv[0];

    struct dirent *dp;
    DIR *dir = opendir(".");

    uint8_t __directories = 0;
    uint8_t __files = 0;

    if (NULL == dir) {
        fprintf(stdout, "Directory not found. Exit\n");
        return (1);
    }


    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_type == 8) { // is a file?
            if (dp->d_name[0] == '.')
                continue;

            if (!get_filename_ext(dp->d_name)) // avoid files without extension
                continue;

            // Print only markdown files
            if (strcmp(get_filename_ext(dp->d_name), "md") == 0) {
                // Read and output the content of file
                FILE *fp = fopen(dp->d_name, "rb");
                char chr;
                while ((chr = getc(fp)) != EOF) {
                    fprintf(stdout, "%c", chr);
                }

                fclose(fp);
            }

            __files++;
        }

        if (dp->d_type == 4) { // found a directory

            if (strcmp(".", dp->d_name) == 0
                || strcmp("..", dp->d_name) == 0
                || dp->d_name[0] == '.')
                continue;

            __directories++;

            fprintf(stdout, "%s\n", dp->d_name);
        }
    }

    fprintf(stdout, "Found %d directories and %d files ", __directories, __files);
    closedir(dir);
    return (0);
}