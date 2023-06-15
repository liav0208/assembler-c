#include "first_run.h"
#include "sanitizer.h"
#include "utils.h"

void first_run(char *name)
{
    int line_cnt = 100;
    char line[100];
    int max_line = 1024;
    int has_error = 0;
    char filename[50];
    FILE *fptr;

    strcpy(filename, name);
    strcat(filename, ".am");
    fptr = fopen(filename, "r");

    CHECK_FILE(fptr);

    while (fgets(line, sizeof(line), fptr)) /*Loop over the file until getting EOF*/
    {
        if (!is_empty(line) && !is_comment(line))
        {
            skipSpaces(line);

            if (line_cnt >= max_line)
            {
                printf("File is too long\n");
                exit(0);
            }
            if (is_label(line))
            {
                char label_name[20];
                int cnt;
                cnt = get_label_name(line, label_name); /*
                 printf("\nfor the label: %s the char is: %c\n", label_name, line[cnt]);*/
            }
            else if (is_instruction(line))
            {
            }

            else if (is_extern_or_entry(line))
            {
            }
            else
            {
            }
            line_cnt++;
        }
    }

    if (has_error == 1)
    {
        exit(0);
    }
}