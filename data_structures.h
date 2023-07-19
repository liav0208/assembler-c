#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5
#define FILE_LIMIT_MEMO 1000

typedef struct
{
    unsigned int bits : 12;
} TwelveBitsStruct;

typedef struct node *ptr;
typedef struct node
{
    char label[50];
    unsigned int row;
    ptr next;
} SignTableRow;

typedef struct list *list_ptr;
typedef struct list
{
    char label[50];
    list_ptr next;
} List;

ptr createNode(const char label[], unsigned int row);
void insertNode(ptr *head, const char label[], unsigned int row);
void printList(ptr head);
int labelExists(ptr head, const char label[]);
void freeList(ptr head);
list_ptr createListNode(const char label[]);
void insertListNode(list_ptr *head, const char label[]);
void printList_2(list_ptr head);
int labelExistsInList(list_ptr head, const char label[]);
void freeList2(list_ptr head);

#endif /* DATA_STRUCTURES_H */