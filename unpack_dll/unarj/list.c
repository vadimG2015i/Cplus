/* list.c
This file implaments a linked list.
Written by: Jesse Smith
Date: Dec 13, 2000.
*/

#include <stdio.h>
#include <memory.h>
#include "list.h"

/* This function will create a empty linked list.
"data_size" contians the starting size of each data element of the list.
When using strings, this should contain the maximum size.
*/
struct List *Create_List(int data_size)
{
   struct List *new_list;
   new_list = calloc(1, sizeof(struct List));
   if (!new_list)
      return NULL;
   if (data_size < 1)
      data_size = 1;
   new_list->element_size = data_size;
   return new_list;
}


/* This function deletes the entire linked list.
*/
void Delete_List(struct List *listA)
{
     struct Node *temp, *prev;

     if (!listA)
        return;

     temp = prev = listA->first;
     while (prev)
     {
         temp = prev->next;
         if (prev->data)
             free(prev->data);
         free(prev);
         prev = temp;
     }
     free(listA);
}


/* This fnction creates a node and enters "data" in to the
data field of the node structure.
"list" is the linked list associated with the node.
*/

struct Node *Create_Node(struct List *listA, void *data)
{
   struct Node *new_node;

   if ((!listA) || (!data))
      return NULL;
   new_node = calloc(listA->element_size, 1);
   if (!new_node)
     return NULL;
   new_node->data = calloc(listA->element_size, 1);
   if (!new_node->data)
      return NULL;
   
   memcpy(new_node->data, data, listA->element_size);
   return new_node;
}


/* This deletes the node passed into the function. The listA automatically
closes to keeps things linked. */

void Delete_Node(struct List *listA, struct Node *node)
{
   if ((!node) || (!listA))
      return;
   if (node->prev)
      node->prev->next = node->next;
   /* link next node to the previous */
   if (node->next)
      node->next->prev = node->prev;
   if (node->data)
      free(node->data);
   if (listA->first == node)
      listA->first = NULL;
   free(node);
}


/* This function makes a COPY of the data stored in "node". A pointer
to this copy is returned.
*/
void *Get_Data(struct List *listA, struct Node *node)
{
   char *value;

   if ((!listA) || (!node))
      return NULL;
   value = calloc(listA->element_size, sizeof(char));
   if (!value)
      return NULL;
   memcpy(value, node->data, listA->element_size);
   return value;
}


/* This function sets the current node's "data" field to the
value stored in "data".
*/
void Set_Data(struct List *listA, struct Node *node, void *data)
{
   if ((!listA) || (!node) || (!data))
      return;
   memcpy(node->data, data, listA->element_size);
}


/* This function returns a node with an IDENTICAL data field as
"value". If no match is found, then "NULL" is returned.
*/

struct Node *Find_Match(struct List *listA, void *value)
{
    struct Node *current;
    int found = 0;

    if ((!value) || (!listA))
       return NULL;
    current = listA->first;
    while ((current) && (!found))
    {
       if (memcmp(value, current->data, listA->element_size))
          current = current->next;
       else
          found = 1;
    }
    return current;
}


/* This function finds the first node in the linked listA with a data
field LESS than "value". If no node is found, the function returns "NULL".
*/
struct Node *Find_Less(struct List *listA, void *value)
{
    struct Node *current;
    int found = 0;
    int result;

    if ((!value) || (!listA))
      return NULL;
    current = listA->first;
    while ((current) && (!found))
    {
       result = memcmp(value, current, listA->element_size);
       if (result > 0)
          found = 1;
       else
          current = current->next;
    }
    return current;
}


/* This function finds the first node in the linked listA that is
GREATER than "value". If no valid node is found, the function
returns "NULL".
*/
struct Node *Find_Greater(struct List *listA, void *value)
{
    struct Node *current;
    int found = 0;
    int result;

    if ((!value) || (!listA))
       return NULL;

    current = listA->first;
    while ((current) && (!found))
    {
       result = memcmp(current, value, listA->element_size);
       if (result > 0)
           found = 1;
       else
          current = current->next;
    }
    return current;
}


/* This function adds a node to the START of the linked listA. */

void Add_Node(struct List *listA, struct Node *node)
{
   if ((!listA) || (!node))
      return;

   if (listA->first)
   {
      listA->first->prev = node;
      node->next = listA->first;
   }
   listA->first = node;
}


/* This function appends a new node to the END of the linked list. */
void Append_Node(struct List *listA, struct Node *node)
{
   struct Node *current;

   if ((!listA) || (!node))
      return;

   if (!listA->first)
      listA->first = node;
   else
   {
      current = listA->first;
      while (current->next)
         current = current->next;
      
      current->next = node;
      node->prev = current;
   }
}
   

/* This function inserts the new node into it's ordered place in the
linked list.
*/
void Insert_Node(struct List *listA, struct Node *node)
{
    struct Node *big;
    if ((!listA) || (!node))
       return;

    big = Find_Greater(listA, node->data);
    if (!big)
        Append_Node(listA, node);
    else
    {
        /* link "node" in before "big" */
        node->prev = big->prev;
        big->prev->next = node;
        big->prev = node;
        node->next = big;
    }
}
