#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preassembler.h"
#include "first_run.h"
#include "second_run.h"
#include "data_structures.h"
#include "sanitizer.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	int i;
	FILE *fptr;
	if (argc < 2) /*Check if user provide filename into the command line*/
	{
		fprintf(stderr, "Missing file name in the command line parameters");
		exit(0);
	}

	for (i = 1; i < argc; i++) /*Run over each filename the user provide*/
	{
		char filename[FILE_NAME_MAX_LEN];
		TwelveBitsStruct instruction_arr[FILE_LIMIT_MEMO];
		TwelveBitsStruct data_arr[FILE_LIMIT_MEMO];
		ptr head = NULL, extern_rows_head = NULL, entries_rows_head = NULL;
		list_ptr entries_head = NULL;
		list_ptr extern_head = NULL;
		int IC = 0, DC = 0;

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

		if (second_run(argv[i], &head, instruction_arr, data_arr, &entries_head, &extern_head, &extern_rows_head))
		{
			fprintf(stderr, "There aree some errors in the code\n");
			continue;
		}

		save_entries_with_rows(head, &entries_rows_head, entries_head);

		write_ob_file(argv[i], instruction_arr, data_arr, IC, DC);
		write_ent_ext_file(argv[i], extern_rows_head, ".ext");
		write_ent_ext_file(argv[i], entries_rows_head, ".ent");

		fclose(fptr); /*Close the file*/

		freeList(head);
		freeList(extern_rows_head);
		freeList2(extern_head);
		freeList2(entries_head);
	}

	return 0;
}
