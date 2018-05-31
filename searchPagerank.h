// Written by Aditya Karia, 2017

#ifndef SEARCHPR_H
#define SEARCHPR_H
#include "BSTree.h"
#include "list.h"
#include "Graph.h"
#include "readData.h"

#define MAX_LENGTH 100

void findMatchedUrls(char *words[], int size, List *);

void readPageRank(List *L, int size);

void sortArrayList(List *arrayList, int size);

void printUrls(List *arrayList, int size);

#endif
