#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preassembler.h"
#include "first_run.h"

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
		char filename[50];

		strcpy(filename, argv[i]);
		strcat(filename, ".as");
		fptr = fopen(filename, "r");

		if (!fptr) /*Check if there is any file with that name*/
		{
			fprintf(stderr, "Invalid file name: %s", filename);
			exit(0);
		}

		preassembler(fptr, argv[i]); /*Use the preassmbler function and provide pointer to the file and the filename the use provided*/
		first_run(argv[1]);
	}

	fclose(fptr); /*Close the file*/
	return 0;
}
