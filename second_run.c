#include "second_run.h"
#include "sanitizer.h"
#include "utils.h"

/*Assembler second run*/
int second_run(char *fname, ptr *head, TwelveBitsStruct *instruction_arr, TwelveBitsStruct *data_arr, list_ptr *entries_head, list_ptr *extern_head, ptr *extern_rows_head)
{
    char filename[FILE_NAME_MAX_LEN], line[LINE_MAX_LEN], *token;
    FILE *amfptr;
    int IC = 0, is_err = 0, opcode, source_op_method, target_op_method, memory_line = 100;
    char *source_op, *target_op;

    strcpy(filename, fname);
    strcat(filename, ".am");

    amfptr = fopen(filename, "r+"); /*Create new file for the .am file*/

    CHECK_FILE(amfptr);

    while (fgets(line, sizeof(line), amfptr)) /*Loop over the file*/
    {
        if (!is_comment(line) && !is_empty(line) && !is_data(line) && !is_extern_or_entry(line)) /*Validate that the line is instruction line*/
        {
            token = strtok(line, " ,\t\n");

            if (token[strlen(token) - 1] == ':') /*if the first word is label and if so skip it*/
            {
                token = strtok(NULL, " ,\t\n");
            }

            opcode = get_opcode(token);

            /*Increamend IC by 1 for the first instruction word*/
            IC++;

            /*Check if there is only one operand*/
            if (opcode == 4 || opcode == 5 || opcode == 7 || opcode == 8 || opcode == 9 || opcode == 10 || opcode == 11 || opcode == 13 || opcode == 12)
            {
                target_op = strtok(NULL, " ,\t\n");

                target_op_method = check_addressing_method(target_op);

                if (target_op_method == 1) /*This is number or string*/
                {
                    if (is_valid_int(target_op))
                    {
                        IC++;
                    }
                    else
                    {
                        IC += strlen(target_op) - 1;
                    }
                }
                else if (target_op_method == 5) /*If operand is register*/
                {
                    IC++;
                }
                else /*if operand is label*/
                {
                    int row = 0;

                    if ((row = labelExistsInList(*extern_head, target_op))) /*Check if label is extern*/
                    {
                        instruction_arr[IC].bits = row;
                        insertNode(extern_rows_head, target_op, memory_line + IC);
                        IC++;
                    }
                    else if ((row = labelExists(*head, target_op))) /* check if label is regular label*/
                    {
                        instruction_arr[IC].bits = ((row << 2) + 2);
                        IC++;
                    }
                    else /*Failure in case the label is not listed*/
                    {
                        fprintf(stderr, "invalid argument %s in line: %s\n", target_op, line);
                        is_err = 1;
                    }
                }
            }
            else if (opcode == 0 || opcode == 1 || opcode == 2 || opcode == 3 || opcode == 6) /*Check if there are two operands*/
            {

                source_op = strtok(NULL, " ,\t\n");
                target_op = strtok(NULL, " ,\t\n");

                source_op_method = check_addressing_method(source_op);
                target_op_method = check_addressing_method(target_op);

                if (source_op_method == 5 && target_op_method == 5) /*If both of the operands are registers*/
                {
                    IC++;
                }
                else
                {
                    int row;

                    if (target_op_method == 1) /*This is number or string*/
                    {
                        if (is_valid_int(target_op))
                        {
                            IC++;
                        }
                        else
                        {
                            IC += strlen(target_op) - 1;
                        }
                    }
                    if (source_op_method == 1) /*This is number or string*/
                    {
                        if (is_valid_int(source_op))
                        {
                            IC++;
                        }
                        else
                        {
                            IC += strlen(source_op) - 1;
                        }
                    }
                    if (target_op_method == 5 || source_op_method == 5) /*If one of the operands are register*/
                    {
                        IC++;
                    }
                    if ((row = labelExistsInList(*extern_head, target_op))) /*if target operand is extern*/
                    {
                        instruction_arr[IC].bits = row;
                        insertNode(extern_rows_head, target_op, memory_line + IC);
                        IC++;
                    }
                    if ((row = labelExists(*head, target_op))) /*if target operand is regular*/
                    {
                        instruction_arr[IC].bits = ((row << 2) + 2);
                        IC++;
                    }
                    if ((row = labelExistsInList(*extern_head, source_op))) /*if source operand is extern*/
                    {
                        instruction_arr[IC].bits = row;
                        insertNode(extern_rows_head, target_op, memory_line + IC);
                        IC++;
                    }
                    if ((row = labelExists(*head, source_op))) /*if source operand is regular*/
                    {
                        instruction_arr[IC].bits = ((row << 2) + 2);
                        IC++;
                    }
                    if (source_op_method == 3 && (!labelExistsInList(*extern_head, source_op) && !labelExists(*head, source_op))) /*If source is unknown*/
                    {
                        fprintf(stderr, "invalid argument %s in line: %s\n", source_op, line);
                        is_err = 1;
                    }

                    if ((target_op_method == 3 && (!labelExistsInList(*extern_head, target_op) && !labelExists(*head, target_op)))) /*if target is unknown*/
                    {
                        fprintf(stderr, "invalid argument %s in line: %s\n", target_op, line);
                        is_err = 1;
                    }
                }
            }
            else /*Case there are no operands*/
            {
                IC++;
            }
        }
    }

    return is_err;
}