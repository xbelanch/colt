#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>

#define MAX_LINE_SIZE 1024
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return NULL;
    return dot + 1;
}

// TODO: Maybe linked list instead of recursive approach?
void traverse_recursively_dir(const char *name, int deep) {
    DIR *dirp;
    struct dirent *dirn;

    if (!(dirp = opendir(name))) {
        fprintf(stderr, "ERROR: %d\n", errno);
        return;
    }

    while ((dirn = readdir(dirp)) != NULL) {
        char path[1024];

        if (dirn->d_name[0] == '.')
            continue;

        // TODO: Fix tree list dir output
        if (dirn->d_type == DT_DIR) {
            for (int i = 0; i < (deep<<2); ++i) putchar(' ');
            fprintf(stdout, ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, dirn->d_name);
            snprintf(path, sizeof(path), "%s/%s", name, dirn->d_name);
            traverse_recursively_dir(path, ++deep);
            --deep;
        } else {
            fprintf(stdout, "|-");
            for (int i = 0; i < (deep<<2); ++i) putchar('-');
            fprintf(stdout, " %s(%d)\n", dirn->d_name, deep);
        }
    }
    closedir(dirp);
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    traverse_recursively_dir(".", 0);

    return (0);
}

int main2(int argc, char *argv[])
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
            if (strcmp(get_filename_ext(dp->d_name), "c") == 0) {
                // Read and output the content of file
                FILE *fp = fopen(dp->d_name, "rb");
                fprintf(stdout, "Parse file: %s\n", dp->d_name);
                char *line = malloc(sizeof(char) * MAX_LINE_SIZE);
                while (fgets(line, MAX_LINE_SIZE - 1, fp)) {
                    // Discard blank lines
                    if (line[0] == '\n')
                        continue;
                    // Remove trailing  newline
                    line[strcspn(line, "\n")] = '\0';

                    // Remove spaces or tabs
                    char *d = line; do { d++; } while (*d == ' ');

                    // Print if match some pattern
                    // TODO: It works?
                    char *r = malloc(sizeof(char) * MAX_LINE_SIZE);
                    fprintf(stdout, "%ld - %ld\n", strlen(line), strlen(line));
                    if (strstr(line, "// TODO: ") != NULL) {
                        memcpy(r, d, MAX_LINE_SIZE);
                        fprintf(stdout, "%s\n", r);
                    }

                }
                free(line);
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