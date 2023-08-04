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
int is_data(char *line)
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
    char cpline[LINE_MAX_LEN];
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
    if (label[0] >= 'A' && label[0] <= 'z' && strlen(label) < 31 && opcode(label) == -1 && is_valid_register(label) == 0 && is_alnum_string(label) && strcmp(label, ".entry") != 0 && strcmp(label, ".extern") != 0)
    {
        return 1;
    }
    return 0;
}

int is_valid_register(char registerName[])
{
    if (strcmp(registerName, "@r0") == 0 || strcmp(registerName, "@r1") == 0 || strcmp(registerName, "@r2") == 0 || strcmp(registerName, "@r3") == 0 || strcmp(registerName, "@r4") == 0 || strcmp(registerName, "@r5") == 0 || strcmp(registerName, "@r6") == 0 || strcmp(registerName, "@r7") == 0)
        return 1;
    return 0;
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
char *get_label_name(const char *str)
{
    int i = 0;
    char *word;
    while (str[i] != '\0' && str[i] != ':')
    {
        i++;
    }

    word = (char *)malloc((i + 1) * sizeof(char));
    if (word == NULL)
    {
        /*Error handling if memory allocation fails*/
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strncpy(word, str, i);
    word[i] = '\0';
    return word;
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

void remove_new_line(char *str)
{
    int i, count = 0;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] != '\n')
            str[count++] = str[i];
    }

    str[count] = '\0'; /* mark the end of str */
}

/*Check if string is valid integer*/
int is_valid_int(char *num)
{
    int i;

    if (num[0] != '+' && num[0] != '-' && !isdigit(num[0]))
    {
        return 0;
    }

    for (i = 1; i < strlen(num); i++)
    {
        if (!isdigit(num[i]))
            return 0;
    }

    return 1;
}

/*Check if string is valid string*/
int is_valid_string(char *str)
{
    int i;

    if (str[0] != '"' || str[strlen(str) - 1] != '"')
    {
        return 0;
    }

    for (i = 1; i < strlen(str) - 1; i++)
    {
        if (!isalpha(str[i]) && !isspace(str[i]))
            return 0;
    }

    if (atoi(str) > TEN_BITS_MAX || atoi(str) < TEN_BITS_MIN)
    {
        return 0;
    }
    return 1;
}

/*Check if string contain only alpha numeric character*/
int is_alnum_string(char *str)
{
    int i;

    for (i = 0; i < strlen(str); i++)
    {
        if (!isalnum(str[i]))
            return 0;
    }

    return 1;
}

/*Check the addressing method of operand*/
int check_addressing_method(char *str)
{
    if (str[0] == '@' && is_valid_register(str)) /*If register*/
    {
        return 5;
    }
    else if (is_valid_string(str) || is_valid_int(str)) /*if direct*/
    {
        return 1;
    }
    else /*if label*/
    {
        return 3;
    }
}

/*Add entries with the relevant row in memory to link list*/
int save_entries_with_rows(ptr head, ptr *entries_rows_head, list_ptr entries_head)
{
    ptr current = head;

    while (current != NULL)
    {
        if (labelExistsInList(entries_head, current->label))
            insertNode(entries_rows_head, current->label, current->row);

        current = current->next;
    }

    return 1;
}

/*Validate valid comma seperator*/
int validate_two_operands(const char *line, const char *op)
{
    int op_len = strlen(op), line_len = strlen(line), i, comma_cnt = 0;

    for (i = 0; i <= line_len - op_len; i++) /*skip all words to after opcode*/
    {
        if (strncmp(line + i, op, op_len) == 0)
        {
            break;
        }
    }

    i += op_len;

    /*Skip spaces*/
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    if (line[i] == ',') /*Invalid comma after operator*/
    {
        return 0;
    }

    /*skip first word*/
    while (line[i] != ' ' && line[i] != '\t' && line[i] != ',')
    {
        i++;
    }

    if (line[i] == '\n')
    {
        fprintf(stderr, "Missing operands\n");
        return 0;
    }

    /*Skip spaces*/
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    if (line[i] == ',') /*Check if char is comma*/
    {
        comma_cnt++;
        i++;
    }

    /*Skip spaces*/
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    if (line[i] == ',') /*Check if char is comma for extra comma*/
    {
        comma_cnt++;
        i++;
    }

    return comma_cnt == 1;
}

/* Validate that the line contains numbers separated by commas.
   Returns 1 if the line is valid, 0 otherwise. */
int validate_numbers_separated_by_comma(const char *line)
{
    int i = 0;
    int foundNumber = 0;   /* Indicates whether a number is found */
    int foundComma = 0;    /* Indicates whether a comma is found */
    int lastWasNumber = 0; /* Indicates whether the last character was a number */

    while (line[i] != '\0')
    {
        if (isdigit(line[i]))
        {
            if (lastWasNumber && foundComma)
            {
                /* Two numbers are not allowed to be separated by more than one comma */
                fprintf(stderr, "Invalid format: More than one comma between two numbers\n");
                return 0;
            }
            lastWasNumber = 1;
            foundNumber = 1; /* Number is found */
            foundComma = 0;  /* Reset comma flag */
            while (isdigit(line[i]))
                i++; /* Skip digits */
        }
        else if (line[i] == ' ' || line[i] == '\t')
        {
            /* Skip spaces and tabs */
            i++;
        }
        else if (line[i] == ',')
        {
            if (!foundNumber)
            {
                /* Number is missing before comma */
                fprintf(stderr, "Invalid format: Number is missing before comma\n");
                return 0;
            }
            if (foundComma)
            {
                /* Too many commas */
                fprintf(stderr, "Invalid format: Too many comma\n");
                return 0;
            }
            lastWasNumber = 0;
            foundComma = 1; /* Comma is found */
            i++;            /* Move to the next character after the comma */
        }
        else
        {
            if (line[i] == '-' && foundComma)
            {
                i++;
            }
            else
            {
                return 0;
            }
        }
    }

    if (!foundNumber || !lastWasNumber || foundComma)
    {
        /* Number is missing after comma or last character was a comma */
        fprintf(stderr, "Invalid format: Number is missing after comma or last character was a comma\n");
        return 0;
    }

    /* Line is valid */
    return 1;
}