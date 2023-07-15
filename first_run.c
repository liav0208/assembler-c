#include "first_run.h"
#include "sanitizer.h"
#include "utils.h"
#include "data_structures.h"

int first_run(char *fname, ptr *head, TwelveBitsStruct *instruction_arr, TwelveBitsStruct *data_arr)
{
    char filename[50], line[100];
    FILE *amfptr;
    int DC = 0, IC = 0, line_cnt = 0, memory_place = 100;
    int is_err = 0;
    char *label;
    int i;

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
                if (labelExists(*head, label))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: label %s already exist\n", line_cnt, label);
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
            }
            else /*This line is instruction*/
            {
                if (!validate_save_instuction(line, instruction_arr, &IC))
                {
                    is_err = 1;
                    fprintf(stderr, "line %d: Invalid Instruction command\n", line_cnt);
                }
            }
        }

        line_cnt++;
    }

    for (i = 0; i < DC; i++)
    {
        printf("\nin data_arr line %d is %d\n", i, data_arr[i].bits);
    }

    for (i = 0; i < IC; i++)
    {
        printf("\nin instruction_arr line %d is %d\n", i, instruction_arr[i].bits);
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
            printf("Invalid .string definition");
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

int validate_save_instuction(char line[], TwelveBitsStruct *array, int *cnt)
{
    char *token;
    int opcode, temp_bits = 0;
    TwelveBitsStruct line_bits;

    token = strtok(line, " \t");

    if (token[strlen(token) - 1] == ':')
    {
        token = strtok(NULL, " \t");
    }

    remove_new_line(token);
    opcode = get_opcode(token);

    if (opcode == -1)
    {
        fprintf(stderr, "\nOpcode %s is not valid\n", token);
        return 0;
    }

    if (opcode == 14 || opcode == 15)
    {
        temp_bits = 0;
        temp_bits += (opcode << 5);
        array[*cnt].bits = temp_bits;
        (*cnt)++;
    }
    else
    {
        char *params;

        params = strtok(NULL, "\n");
        printf("\nParams are: %s\n", params);
    }

    /*Check that what left of the line is ok*/
}