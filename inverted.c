// Written by Aditya Karia, 2017
// Creates inverted index of given urls

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "readData.h"

static void printTree(FILE *out, BSTree t);
static List sortUrls(BSTree t);
static void sortUrlsI(List L, Node *a);

int main(int argc, char const *argv[]) {
    // Open the file that the index is going to be written to
    FILE *out = fopen("invertedIndex.txt", "w");

    // Get url List and pass that into GetInvertedList to create BST
    List L = getCollection();
    BSTree t = GetInvertedList(L);

    // Print tree
    printTree(out, t);

    // Free data structures
    freeList(L);
    dropBSTree(t);
    fclose(out);
    return 0;
}

// This function recursively prints the tree from smallest to largest
// Sorts the url list before printing
static void printTree(FILE *out, BSTree t) {
    if (t == NULL) return;   
    printTree(out, t->left);
    fprintf(out, "%s ", t->value);
    t->urls = sortUrls(t);
    showList(out, t->urls);
    fprintf(out, "\n");
    printTree(out, t->right);
}

// Sorts url alphabetically before printing using insertion sort
static List sortUrls(BSTree t) {
    List old = t->urls;
    List L = newList();
    for (Node *curr = old->first; curr != NULL; curr = curr->next) {
        sortUrlsI(L, curr);
    }
    freeList(old);
    return L;
}

static void sortUrlsI(List L, Node *a) {
    if (L->first == NULL) {
        L->first = nodeDup(a);
        L->last = L->first;
        L->size++;
    } else {
        Node *curr = L->first;
        Node *new = nodeDup(a);
        if (strcasecmp(curr->data, new->data) >= 0) {
            new->next = L->first;
            L->first = new;
            L->size++;
            return;
        }
        while (curr->next != NULL) {
            if (strcasecmp(curr->data, new->data) >= 0 && strcasecmp(curr->next->data, new->data) <= 0) {
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
