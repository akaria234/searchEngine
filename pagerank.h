// Written by Aditya Karia, 2017

#ifndef PAGERRANK_H
#define PAGERRANK_H

#include <stdlib.h>
#include <string.h>
#include "readData.h"

void calculatePageRank(Graph g, List L, double d, double diffPR, int maxIterations);

Node *keyToUrl(List L, int key);

int inLinkTotal(Graph g, int array[], int size);

double outLinkTotal(Graph g, int array[], int size);

int inLinkCount(Graph g, int key);

double outLinkCount(Graph g, int key);

double calculateWin(Graph g, int toUrl, int from);

double calculateWout(Graph g, int toUrl, int from);

int findRefPages(Graph g, int array[], int from);

void savePageRanks(Graph g, List L, long double array[]);

#endif
