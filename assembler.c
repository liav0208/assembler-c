#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preassembler.h"
#include "first_run.h"
#include "second_run.h"
#include "data_structures.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	int i;
	FILE *fptr, *objptr, *externptr, *entriesptr;
	if (argc < 2) /*Check if user provide filename into the command line*/
	{
		fprintf(stderr, "Missing file name in the command line parameters");
		exit(0);
	}

	for (i = 1; i < argc; i++) /*Run over each filename the user provide*/
	{
		char filename[50];
		TwelveBitsStruct instruction_arr[FILE_LIMIT_MEMO];
		TwelveBitsStruct data_arr[FILE_LIMIT_MEMO];
		ptr head = NULL;
		list_ptr entries_head = NULL;
		list_ptr extern_head = NULL;
		int IC = 0, DC = 0, ll;

		strcpy(filename, argv[i]);
		strcat(filename, ".as");
		fptr = fopen(filename, "r");

		if (!fptr) /*Check if there is any file with that name*/
		{
			fprintf(stderr, "Invalid file name: %s\n", filename);
			exit(0);
		}

		/*Use the preassmbler function and provide pointer to the file and the filename the use provided*/
		preassembler(fptr, argv[i]);

		if (first_run(argv[i], &head, instruction_arr, data_arr, &entries_head, &extern_head, &IC, &DC))
		{
			fprintf(stderr, "There aree some errors in the code\n");
			continue;
		}

		printf("DC is: %d and IC is: %d\n", DC, IC);

		if (second_run(argv[i], &head, instruction_arr, data_arr, &entries_head, &extern_head))
		{
			fprintf(stderr, "There aree some errors in the code\n");
			continue;
		}

		CREATE_FILE(argv[i], ".o", "w", objptr);

		for (ll = 0; ll < IC; ll++)
		{
			char *line = int12ToBase64(instruction_arr[ll].bits);
			fputs(line, objptr);
			free(line);
		}

		for (ll = 0; ll < DC; ll++)
		{
			char *line = int12ToBase64(data_arr[ll].bits);
			fputs(line, objptr);
			free(line);
		}

		fclose(objptr);
		fclose(fptr); /*Close the file*/

		printf("\n------------------\n");

		printList(head);
		printf("\n------------------\n");
		printList_2(entries_head);
		printf("\n------------------\n");
		printList_2(extern_head);
	}

	return 0;
}
