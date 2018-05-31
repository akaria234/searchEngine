// Written by Aditya Karia, 2017
// Calculates and prints the page rank of given urls

#include <stdio.h>
#include <math.h>
#include "pagerank.h"

int main(int argc, char *argv[]) {
    // Create the list of all urls and the graph from that
    List L = getCollection();
    Graph g = GetGraph(L);

    // If wrong num of arguments are entered, exit
    if (argc != 4) {
        fprintf(stderr, "wrong num of arguments\n");
        return 1;
    }

    // Get values from command-line arguments
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    // Pass args to calculate page rank function
    calculatePageRank(g, L, d, diffPR, maxIterations);

    // Sort the urls according to page rank
    List sorted = sortList(L);

    // Print the sorted list to pagerankList.txt
    FILE *out = fopen("pagerankList.txt", "w");
    for (Node *curr = sorted->first; curr != NULL; curr=curr->next) {
        int outLink = outLinkCount(g, curr->key);
        fprintf(out, "%s, %d, %.7Lf\n", curr->data, outLink, curr->prop);
    }
    freeList(sorted);
    dropGraph(g);
    return 0;
}

// This function calculates the page rank of each url
void calculatePageRank(Graph g, List L, double d, double diffPR, int maxIterations) {
    int nUrls = L->size;

    // Set all pageranks to 1/nUrls
    for (Node *curr = L->first; curr != NULL; curr = curr->next) {
        curr->prop = 1.0/nUrls;
    }

    // This is the main loop which controls how many iterations of the algo to do
    int iteration = 0;
    double diff = diffPR;
    long double prevRanks[g->nV];
    while (iteration < maxIterations && diff >= diffPR) {
        // Calculatare the damping
        long double damping = 1.0*(1 - d)/(1.0*nUrls);

        //Save the previous iteration page ranks to prevRanks. To be used later
        savePageRanks(g, L, prevRanks);

        // calculate the Win and Wout of each page. Multiply by curr pagerank and add Win and Wou to sum1 
        for (Node *curr = L->first; curr != NULL; curr=curr->next) {
            long double sum1 = 0;        
            int toUrl = curr->key;
            for (int i = 0; i < g->nV; i++) {
                if (i == toUrl) continue;
                if (!g->edges[i][toUrl]) continue;
                long double Win = calculateWin(g, toUrl, i);
                long double Wout = calculateWout(g, toUrl, i);
                Node *page = keyToUrl(L, i);
                sum1 += page->prop * Win * Wout;
            }

            // Calculate and save the new page rank
            long double newRank = damping + d*sum1;
            curr->prop = newRank;
        }

        // calculate the new diff using prevRanks
        long double sum2 = 0;
        int count = 0;
        for (Node *curr = L->first; curr != NULL; curr=curr->next) {
            sum2 += fabsl(curr->prop - prevRanks[count]);
            count++;
        }
        diff = sum2;
        iteration++;
    }
}

// Returns a node given a key
Node *keyToUrl(List L, int key) {
    for (Node *curr = L->first; curr != NULL; curr=curr->next) {
        if (curr->key == key) return curr;
    }
    return NULL;
}

// Calculate total num of links coming in
int inLinkTotal(Graph g, int array[], int size) {
    int inLinks = 0;
    int to = 0;
    for (int i = 0; i < size; i++) {
        to = array[i];
        for (int j = 0; j < g->nV; j++) {
            if (g->edges[j][to]) inLinks++;
        }
    }
    return inLinks;
}

// Calculate total num of links going out
double outLinkTotal(Graph g, int array[], int size) {
    double outLink = 0;
    int from = 0;
    int checkLink = 0;
    for (int i = 0; i < size; i++) {
        from = array[i];
        checkLink = 0;
        for (int j = 0; j < g->nV; j++) {
            if (g->edges[from][j]) {
                outLink++;
                checkLink++;
            }
        }
        if (checkLink == 0) {
            outLink = outLink + 0.5;
        }
    }
    return outLink;
}

// Calculate number of links coming in to a particular node of the graph
int inLinkCount(Graph g, int key) {
    int inLink = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[i][key]) inLink++;
    }
    return inLink;
}

// Calculate number of links going out of a particular node of the graph
double outLinkCount(Graph g, int key) {
    double outLink = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[key][i]) outLink++;
    }
    return outLink;
}

// Calculate Win using inLink helper functions
double calculateWin(Graph g, int toUrl, int from) {
    int numerator = inLinkCount(g, toUrl); 
    int refPages[g->nV];
    int count = findRefPages(g, refPages, from);
    int denominator = inLinkTotal(g, refPages, count);
    return 1.0*numerator/denominator;
}

// Calculate Wout using outLink helper functions
double calculateWout(Graph g, int toUrl, int from) {
    double numerator = outLinkCount(g, toUrl);
    if (numerator == 0) numerator = 0.5;
    int refPages[g->nV];
    int count = findRefPages(g, refPages, from);
    double denominator = outLinkTotal(g, refPages, count);
    if (denominator == 0) denominator = 0.5;
    return 1.0*numerator/denominator;
}

// Find the reference pages of a particular not in the graph
int findRefPages(Graph g, int array[], int from) {
    int count = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[from][i]) {
            array[count] = i;
            count++;
        } 
    }
    return count;
}

// Save previous page ranks into a array
void savePageRanks(Graph g, List L, long double array[]) {
    int count = 0;
    for (Node *curr = L->first; curr != NULL; curr=curr->next) {
        array[count] = curr->prop;
        count++;
    }
}
