// Written by Aditya Karia and Harvey Tan, 2017
// Calculates the scaled footrule of multiple rank files given as command line args
// Brute force algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "scaledFootrule.h"

int main(int argc, char *argv[]) {
    // Check number of arguments 
    if (argc <= 1) {
        fprintf(stderr, "wrong num of arguments\n");
        return 1;
    }
    
    // Copy args to 2D array    
    char *files[argc-1];
    for (int i = 1; i < argc; i++) {
        files[i-1] = mystrdup(argv[i]);
    }

    // Create an array of lists to hold the contents of each file
    List ranks[argc-1];
    for (int i = 0; i < argc-1; i++) ranks[i] = newList();

    // Read contents of each file into array of lists
    for (int i = 0; i < argc-1; i++) {
        getRanks(files[i], ranks[i]);
    }

    // Create a list containing only unique elements from all rank files
    List unionList = newList();
    int key = 1;
    for (int i = 0; i < argc-1; i++) {
        key = 1;
        for (Node *curr = ranks[i]->first; curr != NULL; curr=curr->next) {
            if (!isElm(unionList, curr->data)) {
                ListInsert(unionList, curr->data, key);
                key++;
            }
        }
    }

    // Find the max length out of all rank files
    int maxLength = 0;
    for (int i = 0; i < argc-1; i++) {
        if (ranks[i]->size > maxLength) maxLength = ranks[i]->size;
    }

    // Holds P values, this array is permuted
    int pValue[unionList->size];
    for (int i = 1; i <= unionList->size; i++) pValue[i-1] = i;

    // Holds the mininmum P values
    int minP[unionList->size];
    for (int i = 0; i < unionList->size; i++) minP[i] = 0;

    // 2D array to hold scaled footrule values    
    double *SFRValues[argc-1];
    double minFootrule = 1000;
    for (int i = 0; i < argc-1; i++) {
        SFRValues[i] = malloc(maxLength*sizeof(double));
    }

    // Permute p values and calculate scaled footrule values for each permutation
    permute(pValue, 0, unionList->size, minP, SFRValues, argc-1, ranks, &minFootrule, unionList, maxLength);
    
    // print minFootrule
    printf("%lf\n", minFootrule);
    // Print urls according to min p value  
    for (int i = 0; i < unionList->size; i++) {
        Node *curr = unionList->first;
        for (int j = 1; j < minP[i]; j++) {
            curr = curr->next;
        }
        printf("%s\n", curr->data);
    }

    for (int i = 0; i < argc-1; i++) free(SFRValues[i]);
    freeList(unionList);
    for (int i = 0; i < argc-1; i++) freeList(ranks[i]);
    for (int i = 0; i < argc-1; i++) free(files[i]);
    return 0;
}

// Get contents of each rank file
void getRanks(char *filename, List L) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "%s could not be opened\n", filename);
        exit(0);
    }

    char scan[100] = {0};
    int key = 1;
    while (fscanf(f, "%s", scan) != EOF) {
        if (strncmp(scan, "url", 3) == 0) {
            if (!isElm(L, scan)) {
                ListInsert(L, scan, key);
                key++;
            }
        }
    }
    fclose(f);
}

// Swap array values
void swap(int *array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

// Permute p values recursively and calculate scaled footrule values for each permutation
// Algorithm for permute modified from https://stackoverflow.com/questions/9148543/program-to-print-permutations-of-given-elements
void permute(int pVal[], int i, int length, int minP[], double *SFRVal[], int size, List ranks[], double *minFootrule, List unionList, int maxLength) { 
    if (length == i) {
        calculateSFR(pVal, minP, length, SFRVal, size, ranks, minFootrule, unionList, maxLength);
        return;
    }
    int j = i;
    for (j = i; j < length; j++) { 
        swap(pVal, i , j);
        permute(pVal, i+1, length, minP, SFRVal, size, ranks, minFootrule, unionList, maxLength);
        swap(pVal, i , j);
    }
    return;
}

// Calculate scaled footrule according to p values
void calculateSFR(int pVal[], int minP[], int length, double *SFRVal[], int size, List ranks[], double *minFootrule, List unionList, int maxLength) {
    int i = 0;
    // For each unique url
    for (Node *a = unionList->first; a != NULL; a=a->next) {
        for (int j = 0; j < size; j++) {
            // Find position of url in rank file
            int posInRanks = 0;
            for (Node *curr = ranks[j]->first; curr != NULL; curr = curr->next) {
                if (strcmp(curr->data, a->data) == 0) {
                    posInRanks = curr->key;
                    break;
                }
            }
            // If url doesn't exist in rank file, continue
            if (posInRanks == 0) {
                SFRVal[j][i] = 0;
                continue;
            }
            // Calculate SFR using formula
            SFRVal[j][i] = fabs(1.0*posInRanks/ranks[j]->size - 1.0*pVal[i]/length);
        }
        i++;
    }

    // Sum the 2D array of SFR values
    double currFootrule = addSFRArray(SFRVal, ranks, size, maxLength);
    // If lower than previous minFootrule, save minFootrule and p values 
    if (currFootrule < *minFootrule) {
        *minFootrule = currFootrule;
        savePVals(pVal, minP, length);
    }
}

// Sum 2d array
double addSFRArray(double *SFRVal[], List ranks[], int size, int maxLength) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < maxLength; j++) {
            sum += SFRVal[i][j];
        }
    }
    return sum;
}

// Save p values to minP array
void savePVals(int pVal[], int minP[], int size) {
    for (int i = 0; i < size; i++) {
        minP[i] = pVal[i];
    }
}
