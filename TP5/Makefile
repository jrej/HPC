
SRC = histogram.c
OBJ = $(SRC:.c=.o)
TARGET = $(SRC:.c=.x)

all: $(TARGET)

CFLAGS = -O2 -std=gnu99 -g
LDFLAGS = -lpthread

$(OBJ): $(SRC)
	gcc $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(LDFLAGS)


.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ)


