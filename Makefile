# Author: Angelos T. Dimoglis & a bit of help from chat ;)
# AM: csd5078

# Compiler settings
CC = g++
CFLAGS = -g -Ilib -std=c++20

# Target executable
TARGET = alpha_vm.out

# Directory setup
BIN_DIR = bin
SRC_DIRS = src src/execute_functions

# Find all source files
SRCS = $(wildcard src/*.cpp) $(wildcard src/execute_functions/*.cpp)

# Generate object files (all in bin/)
OBJS = $(addprefix $(BIN_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule for src/*.cpp files
$(BIN_DIR)/%.o: src/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for src/execute_functions/*.cpp files
$(BIN_DIR)/%.o: src/execute_functions/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create bin directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(TARGET) $(BIN_DIR)

remake: clean all

.PHONY: all clean remake