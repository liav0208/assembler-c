#ifndef FIRST_RUN_H
#define FIRST_RUN_H

#include "data_structures.h"

#define CHECK_END_OF_LINE(token)                      \
    {                                                 \
        if (token)                                    \
        {                                             \
            fprintf(stderr, "Invalid end of line\n"); \
            return 0;                                 \
        }                                             \
        else                                          \
        {                                             \
            return 1;                                 \
        }                                             \
    }

int first_run(char *fname, ptr *head, TwelveBitsStruct *instruction_arr, TwelveBitsStruct *data_arr, list_ptr *entries_head, list_ptr *extern_head);
int validate_save_data_line(char line[], TwelveBitsStruct *array, int *cnt);
int validate_save_instuction(char line[], TwelveBitsStruct *array, int *cnt, ptr *head, list_ptr *extern_arr);
int handle_one_operand(char *token, TwelveBitsStruct *array, int *cnt, int opcode, int temp_bits);
int handle_two_operand(char *token, TwelveBitsStruct *array, int *cnt, int opcode, int temp_bits);
#endif /* FIRST_RUN_H */