/* list.h
This file contains the headers for a generic linked list.
Written by: Jesse Smith
Date: Dec 13, 2000.
*/

#ifndef LIST_H__
#define LIST_H__

struct Node
{
    char *data;
    struct Node *next;
    struct Node *prev;
};

struct List
{
   int element_size;
   struct Node *first;
};

struct List *Create_List(int data_size);
void Delete_List(struct List *listA);
struct Node *Create_Node(struct List *listA, void *data);
void Delete_Node(struct List *listA, struct Node *temp);
void *Get_Data(struct List *listA, struct Node *node);
void Set_Data(struct List *listA, struct Node *node, void *data);
struct Node *Find_Match(struct List *listA, void *value);
struct Node *Find_Less(struct List *listA, void *value);
struct Node *Find_Greater(struct List *listA, void *value);
void Add_Node(struct List *listA, struct Node *node);
void Append_Node(struct List *listA, struct Node *node);
void Insert_Node(struct List *listA, struct Node *node);

#endif

