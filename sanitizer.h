#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define CHECK_FILE(file)                                                          \
    if (!file)                                                                    \
    {                                                                             \
        fprintf(stderr, "Failed on create .am file due Memory allocation issue"); \
        exit(0);                                                                  \
    }

int opcode(char ch[]);
int is_empty(char *line);
int is_comment(char *line);
int is_instruction(char *line);
int is_label(char *line);
int legal_label(char label[]);
int is_extern_or_entry(char *line);
int get_label_name(const char *str, char *result);
void split_instructions(const char *str);