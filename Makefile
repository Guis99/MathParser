# Compiler and flags (using Xcode's clang++)
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default test file
TEST = test1.cpp

# Output executable
TARGET = $(BUILD_DIR)/$(basename $(notdir $(TEST)))

# Default target
all: $(TARGET)

# Build the executable for the specified test file
$(TARGET): $(OBJS) $(BUILD_DIR)/$(TEST:.cpp=.o)
	@echo "Linking executable: $@"
	$(CXX) $(OBJS) $(BUILD_DIR)/$(TEST:.cpp=.o) -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo "Compiling test file: $<"
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling source file: $<"
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

# Allow specifying the test file
.PHONY: all clean
