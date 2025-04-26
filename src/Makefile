# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g  # Add -g for debugging symbols

# Directory for source files
SRC_DIR = src
# Directory for object files
OBJ_DIR = obj
# Directory for the output binary
BIN_DIR = bin

# Source files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output executable
TARGET = $(BIN_DIR)/unxusd_example

# Default target
all: $(TARGET)

# Rule to create the output binary from object files
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Rule to compile each .cpp file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/unxusdriver.h
	@mkdir -p $(OBJ_DIR)  # Create obj directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the project (remove object files and the executable)
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets (these aren't actual files)
.PHONY: all clean
