CC = gcc
TARGET = main
SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -g -W -Wall -Werror

all: $(TARGET)

$(TARGET): main.o $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) *.o

.PHONY: all clean
