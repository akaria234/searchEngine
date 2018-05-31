// Written by Aditya Karia and Harvey Tan, 2017
// Prints urls depending on number of search terms and tf-idf value

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include "searchTfIdf.h"

int main(int argc, char *argv[]) {
    // Check number of arguments    
    if (argc <= 1) {
        fprintf(stderr, "wrong num of arguments\n");
        return 1;
    }

    //Get total number of urls
    List urlList = getCollection();

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
    List matchedUrls = findUrls(words,argc-1, listArray);

    // Calculate number of words in each url
    calculateUrlSize(urlList);

    // Calculate tfidf for each url
    calculateTfIdf(listArray, matchedUrls, words, argc-1, urlList, urlList->size);

    // Sort according to tfidf and print
    sortTfidf(listArray, argc);
    printTfidf(listArray, argc);

    for (int i = 0; i < argc; i++) {
        freeList(listArray[i]);
    }
    for (int i = 0; i < argc-1; i++) {
        free(words[i]);
    }
    freeList(urlList);
    return 0;
}

// Finds urls containing search terms and save them in listArray according to number of search terms found
List findUrls(char *words[], int size, List *listArray) {
    List new = newList();

    // Counts how many search terms are found in the url        
    int found[size];
    for (int i = 0; i < size; i++) found[i] = 0;

    // This loop scans through invertedIndex and reads into new which urls contain each word
    char scan[MAX_LENGTH] = {0};
    int key = 0;
    for (int i = 0; i < size; i++) {
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
                    for (int j = 0; j < 3; j++) urlCheck[j] = url[j];
                    if (strcmp(urlCheck, "url") != 0) break; 
                    if (!isElm(new, url)) {
                        ListInsert(new, url, key); 
                        key++;
                    }
                }
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
    return new;    
}


// Count the frequency of all search terms in a specific url
void countFrequency(char *url, char *words[], int wordCount[], int size) {
    char *txt = ".txt";
    char *temp = NULL;
    temp = mystrdup(url);
    temp = strcat(temp, txt);
    char scan[MAX_LENGTH] = {0};
    for (int j = 0; j < size; j++) {
        FILE *f = fopen(temp, "r");
        if (f == NULL) {
            fprintf(stderr, "%s could not be opened\n", temp);
            exit(0);
        }
        while (fscanf(f, "%s", scan) != EOF) {
            normalise(scan);
            if (strcasecmp(words[j], scan) == 0) {
                wordCount[j]++;
                // printf("wordCount[%d]=%d\n", j, wordCount[j]);
            }
        }
        fclose(f);
    }
    free(temp);
}

// Count number of urls containing search terms
int countDocs(List matchedUrls, char *word) {
    int count = 0;
    for (Node *curr = matchedUrls->first; curr != NULL; curr=curr->next) {
        char *txt = ".txt";
        char *url = NULL;
        url = mystrdup(curr->data);
        url = strcat(url, txt);
        FILE *f = fopen(url, "r");
        if (f == NULL) {
            fprintf(stderr, "%s could not be opened\n", url);
            exit(0);
        }
        char scan[MAX_LENGTH] = {0};
        while (fscanf(f, "%s", scan) != EOF) {
            normalise(scan);
            if (strcasecmp(word, scan) == 0) {
                count++;
                break;   
            }
        }
        fclose(f);
        free(url);
    }
    return count;
}

// Calculate Tfidf
void calculateTfIdf(List *listArray, List matchedUrls, char *words[], int size, List urlList, int totalDocs) {
    for (int i = 0; i <= size; i++) {
        double tf = 0.0;
        double idf = 0.0;
        double tfIdf = 0.0;
        double sum = 0.0;
        for (Node *curr = listArray[i]->first; curr != NULL; curr=curr->next) {
            sum = 0;
            tf = 0;
            idf = 0;
            tfIdf = 0;
            int totalWc = 0;
            int numDocs = 0;
            for (Node *a = urlList->first; a != NULL; a=a->next) {
                if (strcmp(curr->data, a->data) == 0) {
                    totalWc = a->count;
                    break;
                }
            }
            int wordCount[size];
            for (int k = 0; k < size; k++) wordCount[k] = 0;
            countFrequency(curr->data, words, wordCount, size);

            for (int j = 0; j < size; j++) {
                numDocs = countDocs(matchedUrls, words[j]);
                if (numDocs == 0) continue;
                tf = 1.0*wordCount[j]/totalWc;
                idf = log10(1.0*totalDocs/numDocs);
                tfIdf = tf*idf;
                sum += tfIdf;
            }
            curr->prop = sum;
        }
    }
}

// Calculate word count of each url
void calculateUrlSize(List urlList) {
    char scan[MAX_LENGTH] = {0};
    int size = 0;
    char *txt = ".txt";
    char *url = NULL;
    for (Node *curr = urlList->first; curr != NULL; curr = curr->next) {
        url = mystrdup(curr->data);
        url = strcat(url, txt);
        size = 0;
        FILE *f = fopen(url, "r");
        if (f == NULL) {
            fprintf(stderr, "%s could not be opened\n", url);
            exit(0);
        }
        while (fscanf(f, "%s", scan) != EOF) {
            if (strcmp(scan, "#start") == 0) continue;
            if (strcmp(scan, "#end") == 0) continue;
            if (strcmp(scan, "Section-2") == 0) continue;
            if (strcmp(scan, "Section-1") == 0) continue;
            if (strncmp(scan, "url", 3) == 0) continue;
            size++;
        }
        curr->count = size;
        fclose(f);
    } 
}

// Sort list
void sortTfidf(List *listArray, int size) {
    for (int i = 0; i < size; i++) {
        listArray[i] = sortList(listArray[i]);
    }
}

// Print list
void printTfidf(List *arrayList, int size) {
    int j = 0;
    for (int i = size - 1; i >= 0; i--) {
        if (j == 30) break;
        for (Node *curr = arrayList[i]->first; curr != NULL; curr=curr->next) {
            if (j == 30) break;
            printf("%s %Lf\n", curr->data, curr->prop);
            j++;
        }
    }
}
