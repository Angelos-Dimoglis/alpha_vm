# Author: Angelos T. Dimoglis
# AM: csd5078

# compiler related variables
CC = g++
CFLAGS = -g -Ilib -std=c++20

# target executable
TARGET = alpha_vm.out

# directory variables
BIN_DIR = bin
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

# default target
all: $(TARGET)

# linking step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# compile all .cpp files
$(BIN_DIR)/%.o: src/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(TARGET) $(BIN_DIR) alpha_vm.out

remake:
	make clean && clear && make

.PHONY: all clean remake
