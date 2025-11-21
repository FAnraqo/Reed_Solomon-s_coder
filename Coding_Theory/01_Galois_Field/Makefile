CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -L.
LIBS = -lfunctions

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

LIB = libfunctions.a
PROGRAM = main
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
MAIN_SRC = main.c
MAIN_OBJ = $(OBJ_DIR)/main.o

all: $(PROGRAM)

$(PROGRAM): $(MAIN_OBJ) $(LIB)
	$(CC) $(MAIN_OBJ) $(LDFLAGS) $(LIBS) -o $@

$(MAIN_OBJ): $(MAIN_SRC) $(INCLUDE_DIR)/functions.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/functions.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIB) $(PROGRAM)

rebuild: clean all

.PHONY: all clean rebuild