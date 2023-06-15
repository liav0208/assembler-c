#include "sanitizer.h"

int opcode(char ch[])
{
    if (strcmp(ch, "mov") == 0)
        return 0;
    else if (strcmp(ch, "cmp") == 0)
        return 1;
    else if (strcmp(ch, "add") == 0)
        return 2;
    else if (strcmp(ch, "sub") == 0)
        return 3;
    else if (strcmp(ch, "not") == 0)
        return 4;
    else if (strcmp(ch, "clr") == 0)
        return 5;
    else if (strcmp(ch, "lea") == 0)
        return 6;
    else if (strcmp(ch, "inc") == 0)
        return 7;
    else if (strcmp(ch, "dec") == 0)
        return 8;
    else if (strcmp(ch, "jmp") == 0)
        return 9;
    else if (strcmp(ch, "bne") == 0)
        return 10;
    else if (strcmp(ch, "red") == 0)
        return 11;
    else if (strcmp(ch, "prn") == 0)
        return 12;
    else if (strcmp(ch, "jsr") == 0)
        return 13;
    else if (strcmp(ch, "rts") == 0)
        return 14;
    else if (strcmp(ch, "stop") == 0)
        return 15;
    else
        return -1;
}

/*Check if the line is empty*/
int is_empty(char *line)
{
    char ch = line[0];
    while (ch != '\n' && ch != EOF)
    {
        if (ch != '\t' && ch != ' ')
            return 0;
        ch = *(++line);
    }

    return 1;
}

/*Check if the line is an comment*/
int is_comment(char *line)
{
    char ch = line[0];
    if (ch == ';')
        return 1;
    return 0;
}

/*Check if this is an Instruction line*/
int is_instruction(char *line)
{
    char *res;

    res = strstr(line, ".data") ? strstr(line, ".data") : strstr(line, ".string");
    if (!res)
        return 0;
    return 1;
}

/*Check if this a label declaration*/
int is_label(char *line)
{
    char cpline[100];
    char *firstword;
    int wlen;

    strcpy(cpline, line);
    firstword = strtok(cpline, " ");

    if ((wlen = strlen(firstword)) > 0)
    {
        if (firstword[wlen - 1] == ':')
            return 1;
    }

    return 0;
}

/*Check if the label name is valid*/
int legal_label(char label[])
{
    if (label[0] >= 'A' && label[0] <= 'z' && strlen(label) < 31 && isalnum(label))
    {
        return 1;
    }
    return 0;
}

/*Check if the line is an Entry or Extern delacration*/
int is_extern_or_entry(char *line)
{
    char *res;

    res = strstr(line, ".enrty") ? strstr(line, ".enrty") : strstr(line, "entry");
    if (!res)
        return 0;
    return 1;
}

/*Get the label name*/
int get_label_name(const char *str, char *result)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == ':')
        {
            break;
        }
        result[i] = str[i];
        i++;
    }

    result[i] = '\0';
    return i;
}

/*Split instructions to the end of the file*/
void split_instructions(const char *str)
{
    char line[100];
    FILE *fptr, *temp_instruction_file, *temp_file;

    /*Open the am file and two temp file for the split*/
    fptr = fopen(str, "r+");
    temp_instruction_file = fopen("temp_instruction_file.am", "w");
    temp_file = fopen("temp_file.am", "w");

    CHECK_FILE(fptr);
    CHECK_FILE(temp_instruction_file);
    CHECK_FILE(temp_file);

    while (fgets(line, sizeof(line), fptr)) /*Loop over the file until getting EOF and put the line into the relevant temp file*/
    {
        if (!is_instruction(line))
        {
            fputs(line, temp_file);
        }
        else
        {
            fputs(line, temp_instruction_file);
        }
    }

    fclose(fptr);
    fclose(temp_instruction_file);

    temp_instruction_file = fopen("temp_instruction_file.am", "r");

    CHECK_FILE(temp_instruction_file);

    /*Loop over the file until getting EOF and add instruction to the new file*/
    while (fgets(line, sizeof(line), temp_instruction_file))
    {
        fputs(line, temp_file);
    }

    fclose(temp_file);
    fclose(temp_instruction_file);

    remove(str);
    remove("temp_instruction_file.am");
    rename("temp_file.am", str);
}