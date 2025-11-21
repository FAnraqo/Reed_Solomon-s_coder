CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Wextra
TARGET = generator_example

SRCS = main.c generator_poly.c
OBJS = $(SRCS:.c=.o)
HEADERS = generator_poly.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
