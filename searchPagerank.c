// Written by Aditya Karia and Harvey Tan, 2017
// Prints urls depending on number of matched terms and page rank

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "searchPagerank.h"

int main(int argc, char *argv[]) {
    // Check number of arguments
    if (argc <= 1) {
        fprintf(stderr, "wrong num of arguments\n");
        return 1;
    }

    // Copy args to 2D array
    char *words[argc-1];

    for (int i = 1; i < argc; i++) {
        words[i-1] = mystrdup(argv[i]);
    }

    // Create a array of Lists of size argc to hold urls depending on num of matched terms
    // ie. if url has 1 matched term, it will be held in index 1 of the array
    List listArray[argc];
    for (int i = 0; i < argc; i++) listArray[i] = newList();

    // Find urls containing search terms
    findMatchedUrls(words, argc-1, listArray);

    // Read page ranks into from pagerankList.txt into url
    readPageRank(listArray, argc);

    // Sort list acording to page rank
    sortArrayList(listArray, argc);

    // Print urls
    printUrls(listArray, argc);

    for (int i = 0; i < argc; i++) freeList(listArray[i]);
    for (int i = 0; i < argc-1; i++) free(words[i]);
    return 0;
}

// Finds urls containing search terms
void findMatchedUrls(char *words[], int size, List *listArray) {
    List new = newList();
 
     // Counts how many search terms are found in the url   
    int found[size];
    for (int i = 0; i < size; i++) found[i] = 0;

    // This loop scans through invertedIndex and reads into new which urls contain each word
    char scan[MAX_LENGTH] = {0};
    for (int i = 0; i < size; i++) {
        int key = 0;
        FILE *index = fopen("invertedIndex.txt", "r");
        if (index == NULL) {
            fprintf(stderr, "invertedIndex.txt could not be opened\n");
            exit(0);
        }
        while (fscanf(index, "%s", scan) != EOF) {
            normalise(scan);
            if (strcasecmp(words[i], scan) == 0) {
                char url[MAX_LENGTH] = {0};
                while (fscanf(index, "%s", url) != EOF) {
                    char urlCheck[4] = {0};
                    for (int i = 0; i < 3; i++) urlCheck[i] = url[i];
                    if (strcmp(urlCheck, "url") != 0) break;
                    if (!isElm(new, url)) ListInsert(new, url, key);
                    key++;
                }
                break;
            }
        }  
        fclose(index);
    }

    // This loop scans through the new list, opening each url and finding how many search terms are contained in it
    char *url = NULL;
    char *txt = ".txt";
    for (Node *curr = new->first; curr != NULL; curr=curr->next) {
        int key = 0;
        url = mystrdup(curr->data);
        url = strcat(url, txt);
        FILE *fp = fopen(url, "r");
        if (fp == NULL) {
            fprintf(stderr, "%s could not be opened\n", url);
            exit(0);
        }
        while (fscanf(fp, "%s", scan) != EOF) {
            normalise(scan);
            for (int i = 0; i < size; i++) {
                if (strcasecmp(words[i], scan) != 0) continue;
                found[i] = 1;
            }
            // printf("%s\n", scan);
        }
        fclose(fp);
        // Count the number of search terms found
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (found[i] == 1) count++;
        }

        for (int i = 0; i < size; i++) found[i] = 0;

        // Insert into the list array according to number of search terms
        ListInsert(listArray[count], curr->data, key);
    }
}

// Read page ranks from pagerankList.txt and save them into the list
void readPageRank(List *L, int size) {
    FILE *ranks = fopen("pagerankList.txt", "r");
    if (ranks == NULL) {
        fprintf(stderr, "pagerankList.txt could not be opened\n");
        exit(0);        
    }
    char buf[MAX_LENGTH] = {0};
    char url[10] = {0};
    double pageRank = 2; 
    char outLink[5] = {0};
    while (fgets(buf, MAX_LENGTH, ranks) != NULL) {
        sscanf(buf, "%[^,], %[^,], %lf", url, outLink, &pageRank);
        for (int i = 0; i < size; i++) {
            for (Node *curr = L[i]->first; curr != NULL; curr=curr->next) {
                if (strcmp(curr->data, url) == 0) {
                    curr->prop = pageRank;
                }
            }
        }
    }
    fclose(ranks);
}

// Sort list according the pagerank
void sortArrayList(List *listArray, int size) {
    for (int i = 0; i < size; i++) {
        listArray[i] = sortList(listArray[i]);
    }
}

// Print urls
void printUrls(List *arrayList, int size) {
    int j = 0;
    for (int i = size - 1; i >= 0; i--) {
        if (j == 30) break;
        for (Node *curr = arrayList[i]->first; curr != NULL; curr=curr->next) {
            if (j == 30) break;
            printf("%s\n", curr->data);
            j++;
        }
    }
}
