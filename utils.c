#include "utils.h"

int strcmp_ignore_newline(const char *str1, const char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 > 0 && str1[len1 - 1] == '\n')
    {
        len1--;
    }
    if (len2 > 0 && str2[len2 - 1] == '\n')
    {
        len2--;
    }

    return strncmp(str1, str2, len1 > len2 ? len1 : len2);
}

/*
    skip all the spaces and tabs in the incoming string
    get pointer to string and chage his value.
*/
void skipSpaces(char *str)
{
    int i;
    for (i = 0; str[i] != ' ' && str[i] != '\t'; i++)
    {
        str++;
    }
}

/*
    get pointer to string and transform the string to remove all the white spaces and add \0 at the end
*/
void removeSpaces(char *str)
{
    int i, count = 0;

    for (i = 0; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            str[count++] = str[i];
    }

    str[count] = '\0'; /* mark the end of str */
}

void add_to_symbol_list(symbol_ptr *hptr, char name[], int location, int type)
{
    symbol_ptr temp, p1;

    temp = (symbol_ptr)malloc(sizeof(Symbol_Table));
    if (!temp)
    {
        printf("\n cannot build list \n");
        exit(0);
    }

    temp->location = location;
    strcpy(temp->name, name);
    temp->type = type;
    temp->next = NULL;

    p1 = *hptr;
    if (!p1)
    {
        p1 = temp;
    }
    else
    {
        while (p1->next)
        {
            p1 = p1->next;
        }

        p1 = temp;
    }
}

void add_to_linet_list(line_ptr *hptr, int data, int is_label, char label_name[])
{
    line_ptr temp, p1;

    temp = (line_ptr)malloc(sizeof(Line_Table));
    if (!temp)
    {
        printf("\n cannot build list \n");
        exit(0);
    }

    temp->data = data;
    temp->is_label = is_label;
    strcpy(temp->label_name, label_name);
    temp->next = NULL;

    p1 = *hptr;
    if (!p1)
    {
        p1 = temp;
    }
    else
    {
        while (p1->next)
        {
            p1 = p1->next;
        }

        p1 = temp;
    }
}