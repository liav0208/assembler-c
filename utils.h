#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct symbol *symbol_ptr;
typedef struct linet *line_ptr;

/*struct for symbol table linked list
for types:
1 - extern
2 - entry
*/
typedef struct symbol
{
    char name[20];
    int location;
    int type;
    symbol_ptr next;
} Symbol_Table;

typedef struct linet
{
    int data;
    int is_label;
    char label_name[20];
    line_ptr next;
} Line_Table;

int strcmp_ignore_newline(const char *str1, const char *str2);
void skipSpaces(char *str);
void removeSpaces(char *str);
void add_to_symbol_list(symbol_ptr *ptr, char name[], int location, int type);
void add_to_linet_list(line_ptr *ptr, int data, int is_label, char label_name[]);