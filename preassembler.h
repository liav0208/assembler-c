#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Type definition of Macro that include macro name and content*/
typedef struct macro
{
    char name[10];
    char text[500]; /*Macro lines are limited to 500 characters*/
} Macro;

/*Recive macros array, macros size and macros counter and check if need to realloc the macros to bigger array*/
#define ENLARGE_MACROS(macros, size, counter)                              \
    if (size == counter)                                                   \
    {                                                                      \
        Macro *temp;                                                       \
        size += 5;                                                         \
        temp = (Macro *)realloc(macros, sizeof(Macro) * size);             \
        if (!temp)                                                         \
        {                                                                  \
            fprintf(stderr, "Memory allocation for macros array failed."); \
            exit(0);                                                       \
        }                                                                  \
        macros = temp;                                                     \
    }

void preassembler(FILE *fptr, char *filename);
int is_macro(char name[20], int macroCnt, Macro *macros);
void handle_line(char *line, Macro *macros, int *cnt, int *size, int *ismacro, FILE *fptr);
