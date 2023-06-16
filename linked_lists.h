#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CHECK_ALLOCATION(list)                    \
    if (!list)                                    \
    {                                             \
        printf("\Failed on Memory Allocation\n"); \
        exit(0);                                  \
    }

typedef struct symbol *symbol_ptr;
typedef struct linet *line_ptr;
typedef struct extern_entry *ee_ptr;

#define INSERT_NODE(temp, ptr) \
    if (!ptr)                  \
    {                          \
        ptr = temp;            \
    }                          \
    else                       \
    {                          \
        while (ptr->next)      \
        {                      \
            ptr = ptr->next;   \
        }                      \
        ptr = temp;            \
    }

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

typedef struct extern_entry
{
    char name[20];
    int line;
    ee_ptr next;
} ExternEntry;

void add_to_symbol_list(symbol_ptr *ptr, char name[], int location, int type);
void add_to_linet_list(line_ptr *ptr, int data, int is_label, char label_name[]);
void add_to_ee_list(ee_ptr *hptr, char name[], int line);