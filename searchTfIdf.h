// Written by Aditya Karia, 2017

#ifndef SEARCHTF_H
#define SEARCHTF_H
#include "BSTree.h"
#include "list.h"
#include "Graph.h"
#include "readData.h"

#define MAX_LENGTH 100

List findUrls(char *words[], int size, List *listArray);

void countFrequency(char *url, char *words[], int wordCount[], int size);

void calculateTfIdf(List *listArray, List matchedUrls, char *words[], int size, List urlList, int totalDocs);

void calculateUrlSize(List urlList);

void sortTfidf(List *listArray, int size);

void printTfidf(List *arrayList, int size);



#endif
