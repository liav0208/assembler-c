#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int strcmp_ignore_newline(const char *str1, const char *str2);
void skipSpaces(char *str);
void removeSpaces(char *str);
char *valid_extern_entry(const char *line);