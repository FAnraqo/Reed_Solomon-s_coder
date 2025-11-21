CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -L.
LIBS = -l_berlekamp_massey

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

LIB = lib_berlekamp_massey.a
PROGRAM = test
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
MAIN_SRC = test_berlekamp_massey.c
MAIN_OBJ = $(OBJ_DIR)/test.o

all: $(PROGRAM)

$(PROGRAM): $(MAIN_OBJ) $(LIB)
	$(CC) $(MAIN_OBJ) $(LDFLAGS) $(LIBS) -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIB) $(PROGRAM)

rebuild: clean all

.PHONY: all clean rebuild
