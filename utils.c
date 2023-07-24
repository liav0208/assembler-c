#include "utils.h"
#include "sanitizer.h"

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

char *valid_extern_entry(const char *line)
{
    char linec[100];
    char *token;

    strcpy(linec, line);
    skipSpaces(linec);

    token = strtok(linec, " ");
    if (strcmp(token, "entry") == 0)
    {
    }
    else if (strcmp(token, "extern") == 0)
    {
    }

    return NULL;
}

char *int12ToBase64(int int12)
{
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *base64Num = (char *)malloc(3); /*Allocate memory for 3 characters*/
    if (!base64Num)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    base64Num[0] = base64_chars[(int12 >> 6) & 0x3F];
    base64Num[1] = base64_chars[int12 & 0x3F];
    base64Num[2] = '\n'; /*Next Line*/
    return base64Num;
}