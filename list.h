// List.h - Lists of integers (interface)
// Written by John Shepherd, July 2008
// Modified by Aditya Karia, 2017

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <strings.h>
// External view of List
// Implementation given in List.c

typedef struct ListNode {
    char *data;  // value of this list item
    int key;
    int count;
    long double prop;
    struct ListNode *next;
                // pointer to node containing next element
} Node;

typedef struct ListRep {
    int  size;  // number of elements in list
    struct ListNode *first;
                // node containing first value
    struct ListNode *last;
                // node containing last value
} *List;

// create a new empty List
List newList();

// free up all space associated with list
void freeList(List);

// display list as one integer per line on stdout
void showList(FILE *, List);

// apppend one integer to the end of a list
void ListInsert(List, char *, int);

// delete first occurrence of v from a list
// if v does not occur in List, no effect
void ListDelete(List, char *);

// return number of elements in a list
int ListLength(List);

// make a physical copy of a list
// new list looks identical to original list
List ListCopy(List);

// check whether a list is sorted in ascending order
// returns 0 if list is not sorted, returns non-zero if it is
int ListIsSorted(List);

// check sanity of an List
int ListOK(List);

int isElm(List, char *);

List sortList(List L);

void listInsertSorted(List L, Node *n);

Node *nodeDup(Node *n);

#endif
