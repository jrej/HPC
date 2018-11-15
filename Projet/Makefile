EXEC=FrameDiff
SOURCES= $(wildcard *.c)#nrutil.c vnrutil.c mouvement.c morpho.c morpho_simd.c mouvement_SIMD.c test_mouvement_SIMD.c test_morpho.c#  #nrutil.c mouvement.c test_mouvement.c
OBJECTS=$(SOURCES:.c=.o)
CCFLAGS=-Wall -g -lm -D	ENABLE_BENCHMARK=1
CC=gcc

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

.depend:
	g++ -MM $(SOURCES) > .depend
-include .depend
clean:
	rm -f $(OBJECTS) $(EXEC)

test:
	./FrameDiff
