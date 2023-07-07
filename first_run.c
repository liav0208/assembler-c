#include "first_run.h"
#include "sanitizer.h"
#include "utils.h"
#include "linked_lists.h"

extern ee_ptr entries_h;
extern ee_ptr extern_h;

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
            else if (is_extern_or_entry(line))
            {
                handle_extern_entry_line(line, line_cnt);
            }
            else
            {
                if (is_label(line))
                {
                }
                else if (is_instruction(line))
                {
                }
                else
                {
                }
                line_cnt++;
            }
        }
    }

    if (has_error == 1)
    {
        exit(0);
    }
}

void handle_extern_entry_line(char line[], int line_cnt)
{
    char *token;

    token = strtok(line, " ");

    if (line[0] && line[strlen(line) - 1] == ':')
    {
        token = strtok(NULL, " ");
    }

    if (strcmp(token, ".entry"))
    {
        token = strtok(NULL, " ");
        add_to_ee_list(&entries_h, token, line_cnt);
    }
    else if (strcmp(token, ".extern"))
    {
        token = strtok(NULL, " ");
        add_to_ee_list(&extern_h, token, line_cnt);
    }
    printf("\ntoken is: %s\n", token);
}