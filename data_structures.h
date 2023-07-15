#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5
#define FILE_LIMIT_MEMO 1000

#define VALIDATE_DYNAMIC_ARR(arr)                       \
    {                                                   \
        if (arr == NULL)                                \
        {                                               \
            fprintf(stderr, "Failed on Dynamic array"); \
            exit(0);                                    \
        }                                               \
    }

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

ptr createNode(const char label[], unsigned int row);
void insertNode(ptr *head, const char label[], unsigned int row);
void printList(ptr head);
int labelExists(ptr head, const char label[]);
void freeList(ptr head);

#endif /* DATA_STRUCTURES_H */