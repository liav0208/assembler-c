#include "first_run.h"
#include "sanitizer.h"
#include "utils.h"
#include "data_structures.h"

int first_run(char *fname, ptr *head, TwelveBitsStruct *instruction_arr, TwelveBitsStruct *data_arr, list_ptr *entries_head, list_ptr *extern_head, int *IC, int *DC)
{
    char filename[FILE_NAME_MAX_LEN], line[LINE_MAX_LEN];
    FILE *amfptr;
    int line_cnt = 1, memory_place = 100;
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
                else if (is_extern_or_entry(line))
                {
                    fprintf(stderr, "line %d: This is extern\\entry line, The label will not save\n", line_cnt);
                }
                else if (!legal_label(label))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: label %s is illegal\n", line_cnt, label);
                }
                else
                {
                    insertNode(head, label, memory_place + *IC + *DC);
                }
            }
            if (is_data(line)) /*Check if data declaration*/
            {
                if (!validate_save_data_line(line, data_arr, DC, line_cnt))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: Invalid data command\n", line_cnt);
                }
            }
            else if (is_extern_or_entry(line)) /*Check if entry or extern*/
            {
                char *cmd, *symbol;

                if (is_label(line))
                {
                    cmd = strtok(line, " \t\n");
                    cmd = strtok(NULL, " \t\n");
                }
                else
                {
                    cmd = strtok(line, " \t\n");
                }
                /*Get the cmd and symbol from the line*/
                symbol = strtok(NULL, " \t\n");

                if (!legal_label(symbol)) /*Check if the symbol is legal*/
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
                            fprintf(stderr, "line %d: entry label %s Already declared\n", line_cnt, symbol);
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
                            fprintf(stderr, "line %d: extern label %s Already declared\n", line_cnt, symbol);
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
                if (!validate_save_instuction(line, instruction_arr, IC, head, extern_head, line_cnt))
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
int validate_save_data_line(char line[], TwelveBitsStruct *array, int *cnt, int line_cnt)
{
    char *first_word, *definition;

    first_word = strtok(line, " \t\n");

    if (first_word[strlen(first_word) - 1] == ':')
    {
        first_word = strtok(NULL, " \t\n");
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
            printf("Invalid .string definition in line: %d\n", line_cnt);
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
            if (definition[i] != ',' && definition[i] != '-' && definition[i] != '+' && !isdigit(definition[i]))
            {
                printf("\nInvalid definition of .data in line: %d\n", line_cnt);
                return 0;
            }
        }

        if (!validate_numbers_separated_by_comma(definition))
        {
            fprintf(stderr, "The .data definition in line: %d is invalid and missing seperated commas\n", line_cnt);
            return 0;
        }
        num = strtok(definition, " ,\t\n");
        while (num) /*Loop while there are numbers in the data*/
        {
            converted_num = atoi(num);
            if (converted_num > TWELVE_BITS_MAX || converted_num < TWELVE_BITS_MIN) /*Validated that the number is valid*/
            {
                fprintf(stderr, "\nThe number: %d is invalid in line: %d\n", converted_num, line_cnt);
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

/*
    Validate instruction line and save into the list
*/
int validate_save_instuction(char line[], TwelveBitsStruct *array, int *cnt, ptr *head, list_ptr *extern_arr, int line_cnt)
{
    char *token, operands[LINE_MAX_LEN];
    int opcode, temp_bits = 0;

    strcpy(operands, line);
    token = strtok(line, " \t\n");

    /*Check if the first word is label definition*/
    if (token[strlen(token) - 1] == ':')
    {
        token = strtok(NULL, " \t\n");
    }

    remove_new_line(token);
    opcode = get_opcode(token);

    /*Check if opcode is valid*/
    if (opcode == -1)
    {
        fprintf(stderr, "\nOpcode %s is not valid in line: %d\n", token, line_cnt);
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

    /*for 'not', 'clr', 'inc', 'dec', 'jmp', 'bne', 'red', 'jsr', 'prn'*/
    if (opcode == 4 || opcode == 5 || opcode == 7 || opcode == 8 || opcode == 9 || opcode == 10 || opcode == 11 || opcode == 13 || opcode == 12)
    {
        return handle_one_operand(token, array, cnt, opcode, temp_bits, line_cnt);
    }

    if (opcode == 0 || opcode == 1 || opcode == 2 || opcode == 3 || opcode == 6) /*for 'mov', 'add' and 'sub'*/
    {
        if (!validate_two_operands(operands, token))
        {
            fprintf(stderr, "Invalid amount of commas or argument in line: %d\n", line_cnt);
            return 0;
        }
        return handle_two_operand(token, array, cnt, opcode, temp_bits, line_cnt);
    }

    return 1;
}

/*Handle instructions line that have only one operand*/
int handle_one_operand(char *token, TwelveBitsStruct *array, int *cnt, int opcode, int temp_bits, int line_cnt)
{
    int target_op_method;
    char *target_op;

    target_op = strtok(NULL, " ,\t\n"); /*Get the operand*/

    if (!target_op) /*Validate if there is operand*/
    {
        fprintf(stderr, "Missing target operand in line: %d\n", line_cnt);
        return 0;
    }

    target_op_method = check_addressing_method(target_op); /*Check if the operand is direct, register or label*/
    VALIDATE_ADDRESS_METHOD(target_op_method);
    temp_bits += target_op_method << 2;

    array[*cnt].bits = temp_bits;
    (*cnt)++;

    if (target_op_method == 5) /*If operand is label*/
    {
        array[*cnt].bits = (target_op[2] - '0') << 2;
        (*cnt)++;
    }
    else if (target_op_method == 1) /*If operand is direct*/
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
        fprintf(stderr, "Invalid addressing method in line: %d\n", line_cnt);
        return 0;
    }

    token = strtok(NULL, ""); /*Get thje end of the line*/
    CHECK_END_OF_LINE(token);
}

/*Handle instructions line that have two operand*/
int handle_two_operand(char *token, TwelveBitsStruct *array, int *cnt, int opcode, int temp_bits, int line_cnt)
{
    int source_op_method, target_op_method;
    char *source_op, *target_op;

    /*Get source and target operand*/
    source_op = strtok(NULL, " ,\t\n");
    target_op = strtok(NULL, " ,\t\n");

    if (!target_op || !source_op) /*Validated that we got two operand*/
    {
        fprintf(stderr, "Not enough oprands in line: %d\n", line_cnt);
        return 0;
    }

    /*Get operands addressing methods*/
    target_op_method = check_addressing_method(target_op);
    source_op_method = check_addressing_method(source_op);
    VALIDATE_ADDRESS_METHOD(target_op_method);
    VALIDATE_ADDRESS_METHOD(source_op_method);

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
        fprintf(stderr, "Invalid addressing method in line: %d\n", line_cnt);
        return 0;
    }

    /*Get end of line*/
    token = strtok(NULL, "");
    CHECK_END_OF_LINE(token);
}