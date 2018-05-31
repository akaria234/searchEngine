// List.c - Lists of integers
// Written by John Shepherd, July 2008
// Modified by Aditya Karia, 2017. 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "list.h"
#include "readData.h"

// create a new empty List
List newList()
{
    struct ListRep *L;

    L = malloc(sizeof (struct ListRep));
    assert (L != NULL);
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}

// free up all space associated with list
void freeList(List L)
{
    for (Node *curr = L->first; curr != NULL;) {
        Node *temp = curr;
        curr = curr->next;
        free(temp->data);
        free(temp);
    }

    free(L);
}



// create a new ListNode with value v
// (this function is local to this ADT)
static struct ListNode *newListNode(char *v, int key)
{
    struct ListNode *n;

    n = malloc(sizeof (struct ListNode));
    assert(n != NULL);
    n->data = mystrdup(v);
    n->key = key;
    n->prop = 1;
    n->count = 0;
    n->next = NULL;
    return n;
}

// apppend one integer to the end of a list
void ListInsert(List L, char *v, int key)
{
    struct ListNode *n;

    assert(L != NULL);
    n = newListNode(v, key);
    if (L->first == NULL)
        L->first = L->last = n;
    else {
        L->last->next = n;
        L->last = n;
    }
    L->size++;
}


// delete first occurrence of v from a list
// if v does not occur in List, no effect
void ListDelete(List L, char *v)
{
    struct ListNode *curr, *prev;

    assert(L != NULL);

    // find where v occurs in list
    prev = NULL; curr = L->first;
    while (curr != NULL && curr->data != v) {
        prev = curr;
        curr = curr->next;
    }
    // not found; give up
    if (curr == NULL) return;
    // unlink curr
    if (prev == NULL)
        L->first = curr->next;
    else
        prev->next = curr->next;
    if (L->last == curr)
        L->last = prev;
    L->size--;
    // remove curr
    free(curr);
}

// return number of elements in a list
int ListLength(List L)
{
    assert(L != NULL);
    return L->size;
}

// make a physical copy of a list
// new list looks identical to original list
List ListCopy(List L)
{
    struct ListRep *Lnew;
    struct ListNode *curr;

    Lnew = newList();
    for (curr = L->first; curr != NULL; curr = curr->next)
        ListInsert(Lnew, curr->data, curr->key);
    return Lnew;
}


// check whether a list is sorted in ascending order
// returns 0 if list is not sorted, returns non-zero if it is
int ListIsSorted(List L)
{
    struct ListNode *curr;

    assert(L != NULL);
    // trivial cases, 0 or 1 items
    if (L->size < 2)
        return 1;
    // scan list, looking for out-of-order pair
    for (curr = L->first; curr->next != NULL; curr = curr->next) {
        if (curr->next->data < curr->data)
            return 0;
    }
    // nothing out-of-order, must be sorted
    return 1;
}

// check sanity of an List (for debugging)
int ListOK(List L)
{
    struct ListNode *p;
    int count;

    if (L == NULL)
        return 1;
    if (L->size == 0)
        return (L->first == NULL && L->last == NULL);

    // scan to (but not past) last node
    count = 1; // at least one node
    for (p = L->first; p->next != NULL; p = p->next)
        count++;

    return (count == L->size && p == L->last);
}

// display list as one integer per line to a file
// assume that the file is open for writing
void showList(FILE *out, List L)
{
    struct ListNode *curr;

    assert(L != NULL);
    for (curr = L->first; curr != NULL; curr = curr->next)
        fprintf(out, "%s ", curr->data);
}

int isElm(List L, char *word) {
    for (struct ListNode *curr = L->first; curr != NULL; curr = curr->next) {
        if (strcasecmp(word, curr->data) == 0) return 1;
    }
    return 0;
}


List sortList(List L) {
    List new = newList();
    for (Node *curr = L->first; curr != NULL; curr=curr->next) {
        listInsertSorted(new, curr);
    }
    return new;
}

Node *nodeDup(Node *n) {
    struct ListNode *new;

    new = malloc(sizeof (struct ListNode));
    assert(new != NULL);
    new->data = mystrdup(n->data);
    new->key = n->key;
    new->prop = n->prop;
    new->next = NULL;
    return new;
}

void listInsertSorted(List L, Node *n) {
    if (L->first == NULL) {
        L->first = nodeDup(n);
        L->last = L->first;
        L->size++;
    } else {
        Node *curr = L->first;
        Node *new = nodeDup(n);
        if (curr->prop <= new->prop) {
            new->next = L->first;
            L->first = new;
            L->size++;
            return;
        }
        while (curr->next != NULL) {
            if (curr->prop >= new->prop && curr->next->prop <= new->prop) {
                new->next = curr->next;
                curr->next = new;
                L->size++;
                return;
            }
            curr = curr->next;
        }
        if (curr->next == NULL) {
            curr->next = new;
            L->last = curr->next;
            L->size++;
        }
    }
}
