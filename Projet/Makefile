EXEC=FrameDiff
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CCFLAGS=-Wall -g -lm
CC=gcc

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)
%.o: %.cc
	$(CC) $(CCFLAGS) -c $< -o $@ 

.depend:
	g++ -MM $(SOURCES) > .depend
-include .depend
clean:
	rm -f $(OBJECTS) $(EXEC)

test:
	./FrameDiff
