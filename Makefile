CC=gcc
CFLAGS=-Wall -Werror -std=c11 -g
LIBS=Graph.o readData.o BSTree.o list.o

all: 	inverted.o pagerank.o searchTfIdf.o searchPagerank.o scaledFootrule.o $(LIBS)
	$(CC) $(CFLAGS) -o pagerank pagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -o inverted inverted.o $(LIBS) 
	$(CC) $(CFLAGS) -o searchTfIdf searchTfIdf.o -lm $(LIBS) 
	$(CC) $(CFLAGS) -o searchPagerank searchPagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -o scaledFootrule scaledFootrule.o $(LIBS) 
	
searchTfIdf.o : searchTfIdf.c searchTfIdf.h
inverted.o : inverted.c
pagerank.o : pagerank.c pagerank.h
searchPagerank.o : searchPagerank.c searchPagerank.h
BSTree.o : BSTree.c BSTree.h
Graph.o : Graph.c Graph.h
readData.o: readData.c readData.h
list.o: list.c list.h
scaledFootrule.o: scaledFootrule.c scaledFootrule.h

clean :
	rm -f pageRank inverted searchPagerank searchTfIdf scaledFootrule *.o core
