CC=gcc
CFLAGS=-I.
DEPS = tp.h grafo.h getopt.h
OBJ = main.o tp.o grafo.o getopt.o
EXEC = tp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core