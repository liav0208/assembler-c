#include <string.h>

typedef struct symbol *symbol_ptr;
typedef struct linet *line_ptr;

/*struct for symbol table linked list*/
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