// Written by Aditya Karia and Harvey Tan, 2017
// Various functions that reads given data and creates appropriate data structures
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readData.h"

void normalise(char *word);

//This function return a list of urls in the collection.txt file
List getCollection(void) {
    // create a new list
    List L = newList();
    FILE *collection = fopen("collection.txt", "r");
    if (collection == NULL) {
        fprintf(stderr, "Couldn't open file: collection\n");
        return NULL;
    }

    char url[1000] = {0};
    int key = 0;
    
    // scan through the file and insert url into the list with a corresponding key
    while (fscanf(collection, "%s", url) != EOF) {
        if (!isElm(L, url)) ListInsert(L, url, key);
        key++;
    }
    fclose(collection);
    return L;
}

// This function maps the url to a key and returns the key
// If key is not found, return 0
int urlToKey(List L, char *url) {
    for (Node *curr = L->first; curr != NULL; curr = curr->next) {
        if (strcmp(curr->data, url) == 0) {
            return curr->key;
        }
    }
    return 0;
}

// This function creates a graph of the url List 
// and adds the edges by opening and scanning each url.
// The graph uses each url's associated key to make the graph
Graph GetGraph(List L) {
    Graph g = newGraph(L->size);
    char *txt = ".txt";
    char *url = NULL;
    for (Node *curr = L->first; curr != NULL; curr = curr->next) {
        // add the .txt file extension to the end of url string
        url = mystrdup(curr->data);
        url = strcat(url, txt);
        
        //Open the url
        FILE *fp = fopen(url, "r");
        if (fp == NULL) {
            fprintf(stderr, "Couldn't open file: %s\n", url);
            return NULL;
        }

        char word[1000] = {0};
        
        // Find what key the url corresponds to
        int fromUrl = urlToKey(L, curr->data);

        // Scan through the file
        while (fscanf(fp, "%s", word) != EOF) {
            // if the end of section 1 is reached, break out of the loop
            if (strcmp(word, "#end") == 0) break;
            if (strcmp(word, "#start") == 0) continue;
            if (strcmp(word, "Section-1") == 0) continue;
            // Add edge between the url of the open file and the url found
            insertEdge(g, fromUrl, urlToKey(L, word));
        }
        fclose(fp);
        free(url);
    }
    return g;
}

// This function return a binary search tree containing all the words in the url files
BSTree GetInvertedList(List L) {
    BSTree t = newBSTree(); 
    char *txt = ".txt";
    for (Node *curr = L->first; curr != NULL; curr = curr->next) {
        // add the .txt file extension to the end of url string
        char *url = mystrdup(curr->data);
        strcat(url, txt);

        //Open the file
        FILE *fp = fopen(url, "r");
        if (fp == NULL) {
            fprintf(stderr, "Couldn't open file: %s\n", url);
            return NULL;
        }


        char word[1000] = {0};
        BSTNode *node = NULL;
        int key = 1;
        while (fscanf(fp, "%s", word) != EOF) {
            normalise(word);
            // If it is a header word, continue
            if (strcasecmp(word, "#end") == 0) continue;
            if (strcasecmp(word, "#start") == 0) continue; 
            if (strcasecmp(word, "section-1") == 0) continue;
            if (strcasecmp(word, "section-2") == 0) continue;

            // If the word is a url, continue
            char urlCheck[4] = {0};
            for (int i = 0; i < 3; i++) urlCheck[i] = word[i];
            if (strcmp(urlCheck, "url") == 0) continue;

            // If the word is in BST and the url has not been entered into the list
            // Add current url to list of urls
            if ((node = BSTreeFindR(t, word)) != NULL && !isElm(node->urls, curr->data)) {
                ListInsert(node->urls, curr->data, key);
                key++;
                continue;
            }

            // Inserting into BST
            t = BSTreeInsert(t, word);
            if ((node = BSTreeFindR(t, word)) != NULL && !isElm(node->urls, curr->data)) {
                ListInsert(node->urls, curr->data, key);
            }
            key++;
        }
        free(url);
        fclose(fp);
    }
    return t;
}

void normalise(char *word) {
    // If word starts with a space, move it forward
    while (*word == ' ') {
        word++;
    }
    int length = strlen(word);
    int i = 2;
    // If it ends in spaces, replace with null terminator
    while (word[length - i] == ' ') {
        word[length - i] = '\0';
    }     
    
    length = strlen(word);
    // if the word ends in any punctuation, replace with null terminator
    if (word[length - 1] == '.' || word[length - 1] == ',' || word[length - 1] == ';' || word[length - 1] == '?') {
        word[length - 1] = '\0';
    }
    
    // Change all chars to lower case
    while (*word != '\0') {
        *word = tolower(*word);
        word++;
    }
}

char *mystrdup(char *string) {
    char* new = malloc(strlen(string)+1);
    if (new != NULL) strcpy(new, string);
    return new;
}
