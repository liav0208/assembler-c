#include "data_structures.h"

/*Function to create a new node*/
ptr createNode(const char label[], unsigned int row)
{
    ptr newNode = (ptr)malloc(sizeof(struct node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strncpy(newNode->label, label, sizeof(newNode->label) - 1);
    newNode->row = row;
    newNode->next = NULL;
    return newNode;
}

/*Function to insert a node at the beginning of the list*/
void insertNode(ptr *head, const char label[], unsigned int row)
{
    ptr newNode = createNode(label, row);
    if (newNode == NULL)
    {
        return; /*Error handling*/
    }
    newNode->next = *head;
    *head = newNode;
}

/*Function to print the linked list*/
void printList(ptr head)
{
    ptr current = head;
    while (current != NULL)
    {
        printf("Label: %s, Row: %u\n", current->label, current->row);
        current = current->next;
    }
}

/*Function to free the memory allocated for the linked list*/
void freeList(ptr head)
{
    ptr current = head;
    while (current != NULL)
    {
        ptr temp = current;
        current = current->next;
        free(temp);
    }
}