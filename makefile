assembler: assembler.o preassembler.o utils.o sanitizer.o first_run.o data_structures.o
	gcc assembler.o preassembler.o utils.o sanitizer.o first_run.o data_structures.o -ansi -Wall -pedantic -o assembler
assembler.o: assembler.c preassembler.h first_run.h data_structures.h
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o
preassembler.o: preassembler.c preassembler.h utils.h sanitizer.h
	gcc -c preassembler.c -ansi -Wall -pedantic -o preassembler.o
utils.o: utils.c utils.h sanitizer.h
	gcc -c utils.c -ansi -Wall -pedantic -o utils.o
sanitizer.o: sanitizer.c sanitizer.h
	gcc -c sanitizer.c -ansi -Wall -pedantic -o sanitizer.o
first_run.o: first_run.c first_run.h
	gcc -c first_run.c -ansi -Wall -pedantic -o first_run.o
data_structures.o: data_structures.c data_structures.h
	gcc -c data_structures.c -ansi -Wall -pedantic -o data_structures.o