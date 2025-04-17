CC = g++
CFLAGS = -g -std=c++17 -Iinclude -lglfw3 -lz -lopengl32 -lglu32 -m64 -Wall -Wextra -Werror

# Directories
SRC_DIR = src
LEVEL_DIR = $(SRC_DIR)/level
PHYS_DIR = $(SRC_DIR)/phys
CHAR_DIR = $(SRC_DIR)/character
TILE_DIR = $(SRC_DIR)/tile
PART_DIR = $(SRC_DIR)/particle
OBJ_DIR = build

# Files
C_FILES = $(wildcard $(SRC_DIR)/*.cpp $(LEVEL_DIR)/*.cpp $(PHYS_DIR)/*.cpp $(CHAR_DIR)/*.cpp $(TILE_DIR)/*.cpp $(PART_DIR)/*.cpp)
OBJ_FILES = $(C_FILES:.cpp=.o)

# Build
OBJ = $(OBJ_DIR)/$(notdir $@)
E_OBJ = $(foreach obj,$^,$(OBJ_DIR)/$(notdir $(obj)))

# Output executable
EXEC = Minecraft

# Rule to build the executable
$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $(E_OBJ) $(CFLAGS)

# Rule to compile each .cpp file into a .o file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $(OBJ)

# Clean rule to remove compiled files
clean:
	rm -f $(OBJ_FILES) $(EXEC)

.PHONY: clean