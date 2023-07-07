#include "linked_lists.h"

void add_to_symbol_list(symbol_ptr *hptr, char name[], int location, int type)
{
    symbol_ptr temp, p1;

    temp = (symbol_ptr)malloc(sizeof(Symbol_Table));
    CHECK_ALLOCATION(temp);

    temp->location = location;
    strcpy(temp->name, name);
    temp->type = type;
    temp->next = NULL;

    p1 = *hptr;
    INSERT_NODE(temp, p1);
}

void free_symbol_list(symbol_ptr *hptr)
{
    symbol_ptr p;

    while (*hptr)
    {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p);
    }
}

void add_to_linet_list(line_ptr *hptr, int data, int is_label, char label_name[])
{
    line_ptr temp, p1;

    temp = (line_ptr)malloc(sizeof(Line_Table));
    CHECK_ALLOCATION(temp);

    temp->data = data;
    temp->is_label = is_label;
    strcpy(temp->label_name, label_name);
    temp->next = NULL;

    p1 = *hptr;
    INSERT_NODE(temp, p1);
}

void free_line_list(line_ptr *hptr)
{
    line_ptr p;

    while (*hptr)
    {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p);
    }
}

void add_to_ee_list(ee_ptr *hptr, char name[], int line)
{
    ee_ptr temp, p1;
    temp = (ee_ptr)malloc(sizeof(ExternEntry));
    CHECK_ALLOCATION(temp);

    temp->line = line;
    strcpy(temp->name, name);
    temp->name = next;

    p1 = *hptr;
    INSERT_NODE(temp, p1);
}

void free_ee_list(ee_ptr *hptr)
{
    ee_ptr p;

    while (*hptr)
    {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p);
    }
}
