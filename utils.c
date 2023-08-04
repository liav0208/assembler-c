#include "utils.h"
#include "sanitizer.h"

/*strcmp that ignore \n symbol*/
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

/*Convert 12 bits to base 64 and return the base64 version*/
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

/*Write ob file with the relevant data*/
int write_ob_file(char *fname, TwelveBitsStruct *instructions, TwelveBitsStruct *data, int IC, int DC)
{
    FILE *fptr = NULL;
    char *line;
    int i;

    CREATE_FILE(fname, ".ob", "w", fptr); /*Create the .ob file*/

    for (i = 0; i < IC; i++) /*Loop over the instructions and write them to the file*/
    {
        line = int12ToBase64(instructions[i].bits);
        fputs(line, fptr);
        free(line);
    }

    for (i = 0; i < DC; i++) /*Loop over the data and write them to the file*/
    {
        line = int12ToBase64(data[i].bits);
        fputs(line, fptr);
        free(line);
    }

    fclose(fptr);
    return 1;
}

/*Write into entries or extern file*/
int write_ent_ext_file(char *fname, ptr head, char *ext)
{
    FILE *fptr = NULL;
    ptr current = head;

    if (current != NULL) /*Create the file only if the list not empty*/
    {
        CREATE_FILE(fname, ext, "w", fptr);

        while (current != NULL)
        {
            fprintf(fptr, "%s: %d\n", current->label, current->row);
            current = current->next;
        }

        fclose(fptr);
    }

    return 1;
}