CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude -O2
LDFLAGS = 
LIBS = -lm

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

TEST_ENC = test_rs_encoder
TEST_DEC = test_rs_decoder

LIB = $(BIN_DIR)/librs.a

all: $(LIB) $(BIN_DIR)/$(TEST_ENC) $(BIN_DIR)/$(TEST_DEC)

$(LIB): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $^
	@echo "✓ Библиотека создана"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/$(TEST_ENC): $(TEST_ENC).c $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LIB) $(LDFLAGS) $(LIBS) -o $@

$(BIN_DIR)/$(TEST_DEC): $(TEST_DEC).c $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LIB) $(LDFLAGS) $(LIBS) -o $@

test-enc: $(BIN_DIR)/$(TEST_ENC)
	@$(BIN_DIR)/$(TEST_ENC)

test-dec: $(BIN_DIR)/$(TEST_DEC)
	@$(BIN_DIR)/$(TEST_DEC)

test: test-enc test-dec

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all test test-enc test-dec clean rebuild
