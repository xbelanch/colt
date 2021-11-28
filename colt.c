#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>

#define MAX_LINE_SIZE 1024
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return NULL;
    return dot + 1;
}

#define PUNCTUATION_MARK_SLASH "/"
#define PUNCTUATION_MARK_SLASH_LEN 1

// TODO: Use memcpy instead of strcpy or strcat (@tsoding style)
char *concat_pathnames(char *basename, char *pathname) {
    size_t basename_len = strlen(basename);
    size_t pathname_len = strlen(pathname);
    char *concat_names = malloc(sizeof(char) * (basename_len + pathname_len + PUNCTUATION_MARK_SLASH_LEN));
    assert(concat_names != NULL);
    strcpy(concat_names, basename);
    strcat(concat_names, PUNCTUATION_MARK_SLASH);
    strcat(concat_names, pathname);
    return concat_names;
}

void read_file_given_extension(char *filepath, char *extension) {
    // exit if filename has no extension
    const char *ext = get_filename_ext(filepath);
    if ( ext == NULL)
        return;

    fprintf(stdout, "!%s: %s\n", filepath, ext);

    if (strcmp(get_filename_ext(filepath), extension) == 0) {
        // Read and output the content of file
        FILE *fp = fopen(filepath, "rb");
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
            if (strstr(line, "// TODO: ") != NULL) {
                memcpy(r, d, MAX_LINE_SIZE);
                fprintf(stdout, "%s\n", r);
            }
        }
        free(line);
        fclose(fp);
    }
}

// TODO: Maybe linked list instead of recursive approach?
void traverse_recursively_dir(char *pathname) {
    DIR *dirp;
    struct dirent *dirn;

    if (!(dirp = opendir(pathname))) {
        fprintf(stderr, "ERROR: Could not open directory %s: %s\n", pathname, strerror(errno));
        exit(1);
    }

    errno = 0;

    while ((dirn = readdir(dirp)) != NULL) {
        if (dirn->d_type == DT_DIR) {
            if (dirn->d_name[0] != '.' && strcmp(dirn->d_name, "..") != 0) {
                char *child_path = concat_pathnames(pathname, dirn->d_name);
                traverse_recursively_dir(child_path);
                fprintf(stdout, ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, child_path);
                free(child_path);
            }
        } else {
            // TODO: Do something with files
            char *filepath = concat_pathnames(pathname, dirn->d_name);
            read_file_given_extension(filepath, "c");
            read_file_given_extension(filepath, "md");
        }
    }
    closedir(dirp);
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    traverse_recursively_dir(".");
    return (0);
}
