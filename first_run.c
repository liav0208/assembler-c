#include "first_run.h"
#include "sanitizer.h"
#include "utils.h"
#include "data_structures.h"

int first_run(char *fname, ptr *head, TwelveBitsStruct *instruction_arr, TwelveBitsStruct *data_arr, list_ptr *entries_head, list_ptr *extern_head)
{
    char filename[50], line[100];
    FILE *amfptr;
    int DC = 0, IC = 0, line_cnt = 0, memory_place = 100;
    int is_err = 0;
    char *label;

    strcpy(filename, fname);
    strcat(filename, ".am");

    amfptr = fopen(filename, "r+"); /*Create new file for the .am file*/

    CHECK_FILE(amfptr);

    while (fgets(line, sizeof(line), amfptr)) /*Loop over the file*/
    {

        if (!is_comment(line) && !is_empty(line))
        {
            if (is_label(line)) /*Check if line is label*/
            {
                label = get_label_name(line);
                if (labelExists(*head, label)) /*Check if the symbol already exist*/
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: label %s already exist\n", line_cnt, label);
                }
                else if (!legal_label(label))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: label %s is illegal\n", line_cnt, label);
                }
                else
                {
                    insertNode(head, label, memory_place + IC + DC);
                }
            }
            if (is_data(line)) /*Check if data declaration*/
            {
                if (!validate_save_data_line(line, data_arr, &DC))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: Invalid data command\n", line_cnt);
                }
            }
            else if (is_extern_or_entry(line)) /*Check if entry or extern*/
            {
                char *cmd, *symbol;

                /*Get the cmd and symbol from the line*/
                cmd = strtok(line, " \t\n");
                symbol = strtok(NULL, " \t\n");

                if (!legal_label(symbol)) /*Check if the symbol already exist*/
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: label %s is illegal\n", line_cnt, symbol);
                }
                else
                {
                    if (strcmp(cmd, ".entry") == 0) /*In case the CMD is .entry*/
                    {
                        if (labelExistsInList(*entries_head, symbol)) /*Check if the symbol already exist*/
                        {
                            is_err = 1;
                            fprintf(stderr, "line %d: label %s is illegal\n", line_cnt, symbol);
                        }
                        else
                        {
                            insertListNode(entries_head, symbol);
                        }
                    }
                    else /*In case the CMD is .extern*/
                    {
                        if (labelExistsInList(*extern_head, symbol)) /*Check if the symbol already exist*/
                        {
                            is_err = 1;
                            fprintf(stderr, "line %d: label %s is illegal\n", line_cnt, symbol);
                        }
                        else
                        {
                            insertListNode(extern_head, symbol);
                        }
                    }
                }
            }
            else /*This line is instruction*/
            {
                if (!validate_save_instuction(line, instruction_arr, &IC, head, extern_head))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: Invalid Instruction command\n", line_cnt);
                }
            }
        }

        line_cnt++;
    }

    return is_err;
}

/*
Validate and save data line.
go over the line and validated that the data is fine and split it andd add it to the data array
*/
int validate_save_data_line(char line[], TwelveBitsStruct *array, int *cnt)
{
    char *first_word, *definition;

    first_word = strtok(line, " \t");

    if (first_word[strlen(first_word) - 1] == ':')
    {
        first_word = strtok(NULL, " \t");
    }

    if (strcmp(first_word, ".data") != 0 && strcmp(first_word, ".string") != 0)
    {
        return 0;
    }

    definition = strtok(NULL, "\n");
    removeSpaces(definition);

    if (strcmp(first_word, ".string") == 0) /*In case the data line is '.string'*/
    {
        int i = 1;

        if (definition[0] != '"' || definition[strlen(definition) - 1] != '"') /*Check if the string data is not valid*/
        {
            printf("Invalid .string definition\n");
            return 0;
        }
        while (definition[i] != '\"') /*Loop over the string and save each character into the array*/
        {
            array[*cnt].bits = definition[i];

            (*cnt)++;
            i++;
        }

        array[*cnt].bits = '\0';
        (*cnt)++;
    }
    else /*In case the data line is '.data'*/
    {
        char *num;
        int converted_num;
        int i;

        for (i = 0; i < strlen(definition); i++) /*Loop over the data and validated if the data is ok*/
        {
            if (definition[i] != ',' && definition[i] != '-' && !isdigit(definition[i]))
            {
                printf("\nInvalid definition of .data\n");
                return 0;
            }
        }

        num = strtok(definition, " ,\t\n");
        while (num)
        {
            converted_num = atoi(num);
            if (converted_num > TWELVE_BITS_MAX || converted_num < TWELVE_BITS_MIN)
            {
                fprintf(stderr, "\nThe number: %d is invalid\n", converted_num);
                return 0;
            }
            else
            {
                array[*cnt].bits = converted_num;
                (*cnt)++;
                num = strtok(NULL, " ,\t\n");
            }
        }
    }

    return 1;
}

int validate_save_instuction(char line[], TwelveBitsStruct *array, int *cnt, ptr *head, list_ptr *extern_arr)
{
    char *token;
    int opcode, temp_bits = 0;

    token = strtok(line, " \t");

    /*Check if the first word is label definition*/
    if (token[strlen(token) - 1] == ':')
    {
        token = strtok(NULL, " \t");
    }

    remove_new_line(token);
    opcode = get_opcode(token);

    /*Check if opcode is valid*/
    if (opcode == -1)
    {
        fprintf(stderr, "\nOpcode %s is not valid\n", token);
        return 0;
    }

    temp_bits += (opcode << 5);

    if (opcode == 14 || opcode == 15) /*Check if 'rts' or 'stop'*/
    {
        array[*cnt].bits = temp_bits;
        (*cnt)++;

        token = strtok(NULL, "");

        /*Validated the the line not contain any more characters*/
        CHECK_END_OF_LINE(token);
    }

    if (opcode == 4 || opcode == 5 || opcode == 7 || opcode == 8 || opcode == 9 || opcode == 10 || opcode == 11 || opcode == 13 || opcode == 12)
    {
        return handle_one_operand(token, array, cnt, opcode, temp_bits);
    }

    if (opcode == 0 || opcode == 1 || opcode == 2 || opcode == 3 || opcode == 6) /*for 'mov', 'add' and 'sub'*/
    {
        return handle_two_operand(token, array, cnt, opcode, temp_bits);
    }

    return 1;
}

int handle_one_operand(char *token, TwelveBitsStruct *array, int *cnt, int opcode, int temp_bits)
{
    int target_op_method;
    char *target_op;

    target_op = strtok(NULL, " ,\t\n");

    if (!target_op)
    {
        fprintf(stderr, "Missing target operand");
        return 0;
    }

    target_op_method = check_addressing_method(target_op);
    temp_bits += target_op_method << 2;

    array[*cnt].bits = temp_bits;
    (*cnt)++;

    if (target_op_method == 5)
    {
        array[*cnt].bits = (target_op[2] - '0') << 2;
        (*cnt)++;
    }
    else if (target_op_method == 1)
    {
        if (is_valid_int(target_op)) /*If target op is int*/
        {
            array[*cnt].bits = atoi(target_op) << 2;
            (*cnt)++;
        }
        else /*Case the target op is string*/
        {
            int i;
            for (i = 1; i < (strlen(target_op) - 1); i++)
            {
                array[*cnt].bits = target_op[i] << 2;
                (*cnt)++;
            }
        }
    }
    else /*Case the value is label*/
    {
        array[*cnt].bits = 1; /*This will act as NULL until the second run*/
        (*cnt)++;
    }

    if (target_op_method == 1 && opcode != 12)
    {
        fprintf(stderr, "Invalid addressing method");
        return 0;
    }

    token = strtok(NULL, "");
    CHECK_END_OF_LINE(token);
}

int handle_two_operand(char *token, TwelveBitsStruct *array, int *cnt, int opcode, int temp_bits)
{
    int source_op_method, target_op_method;
    char *source_op, *target_op;

    source_op = strtok(NULL, " ,\t\n");
    target_op = strtok(NULL, " ,\t\n");

    if (!target_op || !source_op)
    {
        fprintf(stderr, "Not enough oprands");
        return 0;
    }

    target_op_method = check_addressing_method(target_op);
    source_op_method = check_addressing_method(source_op);
    temp_bits += target_op_method << 2;
    temp_bits += source_op_method << 9;

    array[*cnt].bits = temp_bits;
    (*cnt)++;

    if (target_op_method == 5 && source_op_method == 5)
    {
        array[*cnt].bits = (((target_op[2] - '0') << 2) + ((source_op[2] - '0') << 7));
        (*cnt)++;
    }
    else
    {
        if (target_op_method == 5)
        {
            array[*cnt].bits = ((target_op[2] - '0') << 2);
            (*cnt)++;
        }
        if (source_op_method == 5)
        {
            array[*cnt].bits = ((source_op[2] - '0') << 7);
            (*cnt)++;
        }
        if (target_op_method == 1)
        {
            if (is_valid_int(target_op)) /*If target op is int*/
            {
                array[*cnt].bits = atoi(target_op) << 2;
                (*cnt)++;
            }
            else /*Case the target op is string*/
            {
                int i;
                for (i = 1; i < (strlen(target_op) - 1); i++)
                {
                    array[*cnt].bits = target_op[i] << 2;
                    (*cnt)++;
                }
            }
        }
        if (source_op_method == 1)
        {
            if (is_valid_int(source_op)) /*If target op is int*/
            {
                array[*cnt].bits = atoi(source_op) << 2;
                (*cnt)++;
            }
            else /*Case the target op is string*/
            {
                int i;
                for (i = 1; i < (strlen(source_op) - 1); i++)
                {
                    array[*cnt].bits = source_op[i] << 2;
                    (*cnt)++;
                }
            }
        }
        if (target_op_method == 3)
        {
            array[*cnt].bits = 1; /*This will act as NULL until the second run*/
            (*cnt)++;
        }
        if (source_op_method == 3)
        {
            array[*cnt].bits = 1; /*This will act as NULL until the second run*/
            (*cnt)++;
        }
    }

    /*Validated that all operading addressing methods are valid*/
    if ((opcode == 0 && target_op_method == 1) || (opcode == 2 && target_op_method == 1) || (opcode == 3 && target_op_method == 1) || (opcode == 6 && target_op_method == 1) || (opcode == 6 && source_op_method == 1) || (opcode == 6 && source_op_method == 5))
    {
        fprintf(stderr, "Invalid addressing method");
        return 0;
    }

    token = strtok(NULL, "");
    CHECK_END_OF_LINE(token);
}