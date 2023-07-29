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
    strcpy(newNode->label, label);
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
        /* Error handling */
        return;
    }

    if (*head == NULL) /*The linked list is empty*/
    {
        *head = newNode; /*Update the head to point to the new node*/
    }
    else
    {
        ptr current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
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

/*Function to check if a label already exists in the list*/
int labelExists(ptr head, const char label[])
{
    ptr current = head;
    while (current != NULL)
    {
        if (strcmp(current->label, label) == 0)
        {
            return current->row; /*Label already exists*/
        }
        current = current->next;
    }
    return 0; /*Label does not exist*/
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

/*Function to create a new node*/
list_ptr createListNode(const char label[])
{
    list_ptr newListNode = (list_ptr)malloc(sizeof(struct list));
    if (newListNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strcpy(newListNode->label, label);
    newListNode->next = NULL;
    return newListNode;
}

/*Function to insert a node at the beginning of the list*/
void insertListNode(list_ptr *head, const char label[])
{
    list_ptr newNode = createListNode(label);

    if (newNode == NULL)
    {
        /* Error handling */
        return;
    }

    if (*head == NULL) /*The linked list is empty*/
    {
        *head = newNode; /*Update the head to point to the new node*/
    }
    else
    {
        list_ptr current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*Function to print the linked list*/
void printList_2(list_ptr head)
{
    list_ptr current = head;
    while (current != NULL)
    {
        printf("Label: %s\n", current->label);
        current = current->next;
    }
}

/*Function to check if a label already exists in the list*/
int labelExistsInList(list_ptr head, const char label[])
{
    list_ptr current = head;
    while (current != NULL)
    {
        if (strcmp(current->label, label) == 0)
        {
            return 1; /*Label already exists*/
        }
        current = current->next;
    }
    return 0; /*Label does not exist*/
}

/*Function to free the memory allocated for the linked list*/
void freeList2(list_ptr head)
{
    list_ptr current = head;
    while (current != NULL)
    {
        list_ptr temp = current;
        current = current->next;
        free(temp);
    }
}
