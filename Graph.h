// Graph.h ... interface to Graph ADT
// Written by John Shepherd, March 2013
// Modified by Aditya Karia, 2017

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

// graph representation is hidden
typedef struct GraphRep *Graph;

// graph representation (adjacency matrix)
typedef struct GraphRep {
    int    nV;    // #vertices
    int    nE;    // #edges
    int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int   validV(Graph,Vertex); //validity check

// edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex w; } Edge;
void insertEdge(Graph, Vertex, Vertex);
void removeEdge(Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph(int nV);
void dropGraph(Graph);
Graph makeGraph(int, int**);
void showGraph(Graph);
int  findPath(Graph, Vertex, Vertex, int, int *);
int isEdge(Graph, Vertex, Vertex);

#endif
