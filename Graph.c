// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013
// Modified by Aditya Karia, 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
// #include "Queue.h"


// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	Edge new = {v,w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (v == w) return;
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = 1; //outgoing link
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	int i;
   	for (i = 0; i < g->nV; i++)
    	free(g->edges[i]);
   	free(g->edges);
   	free(g);

}

// display graph, using names for vertices
void showGraph(Graph g)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("1");
			} else {
				printf("0");
			}
		}
		printf("\n");
	}
}

int isEdge(Graph g, Vertex v, Vertex w) {
	if (g->edges[v][w]) return 1;
	return 0;
}
