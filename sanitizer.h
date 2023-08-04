#ifndef SANITIZER_H
#define SANITIZER_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "data_structures.h"

#define TEN_BITS_MAX 511
#define TEN_BITS_MIN -512
#define TWELVE_BITS_MAX 2047
#define TWELVE_BITS_MIN -2048
#define LINE_MAX_LEN 100
#define FILE_NAME_MAX_LEN 50

/*Macro for checking that file created successfully*/
#define CHECK_FILE(file)                                                          \
    if (!file)                                                                    \
    {                                                                             \
        fprintf(stderr, "Failed on create .am file due Memory allocation issue"); \
        exit(0);                                                                  \
    }

int opcode(char ch[]);
int is_empty(char *line);
int is_comment(char *line);
int is_data(char *line);
int is_label(char *line);
int legal_label(char label[]);
int is_extern_or_entry(char *line);
char *get_label_name(const char *str);
int is_valid_register(char register[]);
int get_opcode(const char opcode[]);
void remove_new_line(char *str);
int is_valid_int(char *num);
int is_valid_string(char *str);
int is_alnum_string(char *str);
int check_addressing_method(char *str);
int save_entries_with_rows(ptr head, ptr *entries_rows_head, list_ptr entries_head);
int validate_two_operands(const char *line, const char *op);
int validate_numbers_separated_by_comma(const char *line);
#endif