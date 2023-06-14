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
