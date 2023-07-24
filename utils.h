#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CREATE_FILE(name, ext, permission, ptr)                   \
    {                                                             \
        char filename[50];                                        \
        strcpy(filename, name);                                   \
        strcat(filename, ext);                                    \
        ptr = fopen(filename, permission);                        \
        if (!ptr) /* Check if there is any file with that name */ \
        {                                                         \
            fprintf(stderr, "Invalid file name: %s\n", filename); \
            exit(0);                                              \
        }                                                         \
    }

int strcmp_ignore_newline(const char *str1, const char *str2);
void skipSpaces(char *str);
void removeSpaces(char *str);
char *valid_extern_entry(const char *line);
char *int12ToBase64(int int12);