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
    if (label[0] >= 'A' && label[0] <= 'z' && strlen(label) < 31 && isalnum(label) && opcode(label) == -1 && is_valid_register(label) == -1)
    {
        return 1;
    }
    return 0;
}

int is_valid_register(char registerName[])
{
    if (strcmp(registerName, "@r0") == 0 || strcmp(registerName, "@r1") == 0 || strcmp(registerName, "@r2") == 0 || strcmp(registerName, "@r3") == 0 || strcmp(registerName, "@r4") == 0 || strcmp(registerName, "@r5") == 0 || strcmp(registerName, "@r6") == 0 || strcmp(registerName, "@r7") == 0)
        return 1;
    return -1;
}

/*Check if the line is an Entry or Extern delacration*/
int is_extern_or_entry(char *line)
{
    char *res;

    res = strstr(line, ".entry") ? strstr(line, ".entry") : strstr(line, ".extern");
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

int get_opcode(const char opcode[])
{
    if (strcmp(opcode, "mov") == 0)
    {
        return 0;
    }
    else if (strcmp(opcode, "cmp") == 0)
    {
        return 1;
    }
    else if (strcmp(opcode, "add") == 0)
    {
        return 2;
    }
    else if (strcmp(opcode, "sub") == 0)
    {
        return 3;
    }
    else if (strcmp(opcode, "not") == 0)
    {
        return 4;
    }
    else if (strcmp(opcode, "clr") == 0)
    {
        return 5;
    }
    else if (strcmp(opcode, "lea") == 0)
    {
        return 6;
    }
    else if (strcmp(opcode, "inc") == 0)
    {
        return 7;
    }
    else if (strcmp(opcode, "dec") == 0)
    {
        return 8;
    }
    else if (strcmp(opcode, "jmp") == 0)
    {
        return 9;
    }
    else if (strcmp(opcode, "bne") == 0)
    {
        return 10;
    }
    else if (strcmp(opcode, "red") == 0)
    {
        return 11;
    }
    else if (strcmp(opcode, "prn") == 0)
    {
        return 12;
    }
    else if (strcmp(opcode, "jsr") == 0)
    {
        return 13;
    }
    else if (strcmp(opcode, "rts") == 0)
    {
        return 14;
    }
    else if (strcmp(opcode, "stop") == 0)
    {
        return 15;
    }
    else
    {
        return -1;
    }
}