#ifndef READDATA_H
#define READDATA_H

#include "list.h"
#include "Graph.h"
#include "BSTree.h"

List getCollection(void);

Graph GetGraph(List L);

BSTree GetInvertedList(List L);

int urlToKey(List, char *);

void normalise(char *);

char *mystrdup(char *s);

#endif
