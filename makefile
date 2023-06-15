assembler: assembler.o preassembler.o utils.o sanitizer.o first_run.o
	gcc assembler.o preassembler.o utils.o sanitizer.o first_run.o -ansi -Wall -pedantic -o assembler
assembler.o: assembler.c preassembler.h
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o
preassembler.o: preassembler.c preassembler.h utils.h sanitizer.h
	gcc -c preassembler.c -ansi -Wall -pedantic -o preassembler.o
utils.o: utils.c utils.h
	gcc -c utils.c -ansi -Wall -pedantic -o utils.o
sanitizer.o: sanitizer.c sanitizer.h
	gcc -c sanitizer.c -ansi -Wall -pedantic -o sanitizer.o
first_run.o: first_run.c first_run.h sanitizer.h
	gcc -c first_run.c -ansi -Wall -pedantic -o first_run.o