#ifndef FIRST_RUN_H
#define FIRST_RUN_H

#include <string.h>
#include "data_structures.h"

int first_run(char *fname, ptr *head, TwelveBitsStruct *instruction_arr, TwelveBitsStruct *data_arr);
int validate_save_data_line(char line[], TwelveBitsStruct *array, int *cnt);
int validate_save_instuction(char line[], TwelveBitsStruct *array, int *cnt);

#endif /* FIRST_RUN_H */