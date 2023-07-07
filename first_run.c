#include "first_run.h"
#include "sanitizer.h"

int first_run(char *fname)
{
    char filename[50], line[100];
    FILE *amfptr;
    int DC = 0, IC = 0, is_symbol = 0, line_cnt = 100;
    int is_err = 0;

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
                is_symbol = 1;
            }
            if (is_instruction(line)) /*Check if instruction*/
            {
            }
            else if (is_extern_or_entry(line)) /*Check if entry or extern*/
            {
            }
        }

        is_symbol = 0;
        line_cnt++;
    }

    return 0;
}