MODULE_NAME = Universal_Encoder
MODULE_DIR = $(CURDIR)
INCLUDE_DIR = $(MODULE_DIR)/include
SRC_DIR = $(MODULE_DIR)/src
TEST_DIR = $(MODULE_DIR)/tests

TARGET = $(BIN_DIR)/test_universal_encoder
LIB = $(LIB_DIR)/libencoder.a

SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(TEST_DIR)/test_*.c
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/encoder_%.o, $(SRCS))

CFLAGS += -I$(INCLUDE_DIR) $(INCLUDES)

.PHONY: build test clean info

build: $(LIB)
	@echo "✓ $(MODULE_NAME) library built: $(LIB)"

test: $(TARGET)
	@echo "Running $(MODULE_NAME) tests..."
	@$(TARGET)

$(TARGET): $(TEST_SRCS) $(OBJS) $(LIB_DIR)/libgf.a $(LIB_DIR)/libpolygen.a
	@mkdir -p $(BIN_DIR)
	@echo "  Compiling tests for $(MODULE_NAME)..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "  Test executable: $@"

$(LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	@echo "  Creating library $(MODULE_NAME)..."
	@ar rcs $@ $^

$(OBJ_DIR)/encoder_%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "  Compiling: $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning $(MODULE_NAME)..."
	@rm -f $(TARGET) $(LIB) $(OBJS)

info:
	@echo "=== $(MODULE_NAME) Module ==="
	@echo "Include:  $(INCLUDE_DIR)"
	@echo "Source:   $(SRC_DIR)"
	@echo "Tests:    $(TEST_DIR)"
	@echo "Library:  $(LIB)"
	@echo "Target:   $(TARGET)"
	@echo "Dependencies: libgf.a, libpolygen.a"