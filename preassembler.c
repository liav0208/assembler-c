#include "preassembler.h"
#include "utils.h"
#include "sanitizer.h"

/*preassembler function - get pointer to file and pointer to string as filename
Create dynamic array of macros and analyze the file line by line
get all macros and save them and crete new file with .am extension that hold the
original file wioth open macros.
*/
void preassembler(FILE *fptr, char *filename)
{
    char line[100];
    FILE *amfptr;
    char amfname[50];
    int macros_size = 5, macros_counter = 0, ismacro = 0;
    Macro *macros = (Macro *)malloc(sizeof(Macro) * macros_size); /*Create dynasmic macros array*/

    if (!macros) /*Check if there was any memory allocation failure for the macros array*/
    {
        fprintf(stderr, "Memory allocation for macros array failed.");
        exit(0);
    }

    strcpy(amfname, filename);
    strcat(amfname, ".am");
    amfptr = fopen(amfname, "w"); /*Create new file for the .am file*/

    CHECK_FILE(amfptr);

    while (fgets(line, sizeof(line), fptr)) /*Loop over the file until getting EOF*/
    {
        if (!is_comment(line) && !is_empty(line))
            handle_line(line, macros, &macros_counter, &macros_size, &ismacro, amfptr); /*Analyze each line*/
    }

    free(macros); /*Free the macros array memory*/

    fclose(amfptr);

    /*split_instructions(amfname);*/
}

/*Get name, macro list and macro counter and check if the current name is a macro*/
int is_macro(char name[20], int macroCnt, Macro *macros)
{
    int i;
    for (i = 0; i < macroCnt; i++)
    {
        if (strcmp_ignore_newline(macros[i].name, name) == 0) /*check if the current name is a mcro*/
        {
            return i;
        }
    }
    return -1;
}

/*Handle Line function - get line string, macros array and size and counter and pointer to file
    for each line check if there are macro definition, macro usage or just a line that need to be move to the .am file
*/
void handle_line(char *line, Macro *macros, int *cnt, int *size, int *ismacro, FILE *fptr)
{
    char linecopy[100];
    char *firstword;
    int macroCnt = 0;

    strcpy(linecopy, line);
    firstword = strtok(linecopy, " ");
    if (*ismacro) /*If the macro flag is 1*/
    {
        if (strcmp(firstword, "endmcro\n") == 0)
        {
            *ismacro = 0;
            (*cnt)++;
            ENLARGE_MACROS(macros, *size, *cnt);
        }
        else
        {
            strcat(macros[*cnt].text, line);
        }
    }
    else if (strcmp(firstword, "mcro") == 0) /*if this line is mcro*/
    {
        *ismacro = 1;
        strcpy(macros[*cnt].name, strtok(NULL, " "));
    }
    else if ((macroCnt = is_macro(firstword, *cnt, macros)) != -1) /*In case the line contain mcro name*/
    {
        fputs(macros[macroCnt].text, fptr);
    }
    else /*in case that there is a regular line (not macro and macro flag is off)*/
    {
        fputs(line, fptr);
    }
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