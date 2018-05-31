// Written by Aditya Karia, 2017

#ifndef SCALEDFOOTRULE_H
#define SCALEDFOOTRULE_H

#include "list.h"
#include "readData.h"

void getRanks(char *filename, List L);

void swap(int *array, int i, int j);

void permute(int pVal[], int i, int length, int minP[], double *SFRVal[], int size, List ranks[], double *minFootrule, List unionList, int maxLength);

void calculateSFR(int pVal[], int minP[], int length, double *SFRVal[], int size, List ranks[], double *minFootrule, List unionList, int maxLength);

double addSFRArray(double *SFRVal[], List ranks[], int size, int maxLength);

void savePVals(int pVal[], int minP[], int size);

#endif
