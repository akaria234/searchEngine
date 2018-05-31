// BSTree.h ... interface to binary search tree ADT
// Written by John Shepherd, March 2013
// Modified by Aditya Karia, 2017

#ifndef BSTREE_H
#define BSTREE_H
#include "list.h"

typedef struct BSTNode *Link;

typedef struct BSTNode {
    char *value;
    List urls;
    Link left, right;
} BSTNode;

typedef struct BSTNode *BSTree;

typedef void (*Visit)(struct BSTNode *);

// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display a BSTree (sideways)
void showBSTree(BSTree);

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree, char *);
BSTree BSTreeInsertI(BSTree, char *);
// delete a value from a BSTree
BSTree BSTreeDelete(BSTree, char *);

// check whether a value is in a BSTree
int BSTreeFind(BSTree, char *);
// check whether a value is in a BSTree
int BSTreeFindI(BSTree, char *);
// compute depth of BSTree
int BSTreeDepth(BSTree);
// count #nodes in BSTree
int BSTreeNumNodes(BSTree);
// returns a pointer to the node
BSTree BSTreeFindR(BSTree t, char *v);

// generic traversal
void BSTreeTraverse(BSTree, Visit, char *);

#endif
