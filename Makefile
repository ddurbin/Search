CC = gcc
CCFLAGS = -Wall -g
ARKEYS = rv
TARGET = search

all: search index
search: mainSearch.o prefixTree.o search.o
	$(CC) $(CCFLAGS) -o search mainSearch.o prefixTree.o search.o
mainSearch.o: mainSearch.c
	$(CC) $(CCFLAGS) -c mainSearch.c
prefixTree.o: prefixTree.c
	$(CC) $(CCFLAGS) -c prefixTree.c
search.o: search.c
	$(CC) $(CCFLAGS) -c search.c
index: mainIndexer.o tokenizer.o hash-table.o traverse_dir.o
	$(CC) $(CCFLAGS) -o index mainIndexer.o tokenizer.o hash-table.o traverse_dir.o
mainIndexer.o: mainIndexer.c
	$(CC) $(CCFLAGS) -c mainIndexer.c
tokenizer.o: tokenizer.c
	$(CC) $(CCFLAGS) -c tokenizer.c
hash-table.o: hash-table.c
	$(CC) $(CCFLAGS) -c hash-table.c
traverse_dir.o: traverse_dir.c
	$(CC) $(CCFLAGS) -c traverse_dir.c                
clean:
	rm -f sl *.o search index
