#ifndef UTILS_H
#define UTILS_H

#include "data_structures.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EMPTY_REMOVE(fptr, head, extern_rows_head, entries_rows_head, entries_head, extern_head, instruction_arr, data_arr) \
    {                                                                                                                       \
        fclose(fptr); /*Close the file*/                                                                                    \
        freeList(head);                                                                                                     \
        freeList(extern_rows_head);                                                                                         \
        freeList(entries_rows_head);                                                                                        \
        freeList2(entries_head);                                                                                            \
        freeList2(extern_head);                                                                                             \
        empty_TB_arr(instruction_arr);                                                                                      \
        empty_TB_arr(data_arr);                                                                                             \
    }

/*Macro definition for creating new file*/
#define CREATE_FILE(name, ext, permission, ptr)                   \
    {                                                             \
        char filename[FILE_NAME_MAX_LEN];                         \
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
void removeSpaces(char *str);
char *int12ToBase64(int int12);
int write_ob_file(char *fname, TwelveBitsStruct *instructions, TwelveBitsStruct *data, int IC, int DC);
int write_ent_ext_file(char *fname, ptr head, char *ext);

#endif