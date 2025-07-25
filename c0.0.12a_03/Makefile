CXX ?= g++
ifeq ($(OS), Windows_NT)
CXXFLAGS += -g -Og -std=c++17 -DWIN -Iinclude -lglfw3 -lz -lopengl32 -lglu32 -m64 -Wall -Wextra -Werror
MKDIR = mkdir
else
CXXFLAGS += -g -Og -std=c++17 -Iinclude -lglfw -lz -lOpenGL -lGLU -m64 -Wall -Wextra -Werror
MKDIR = mkdir -p
endif

# Directories
SRC_DIR = src
CHAR_DIR = $(SRC_DIR)/character
GUI_DIR = $(SRC_DIR)/gui
LEVEL_DIR = $(SRC_DIR)/level
TILE_DIR = $(SRC_DIR)/level/tile
MC_DIR = $(SRC_DIR)/minecraft
PART_DIR = $(SRC_DIR)/particle
PHYS_DIR = $(SRC_DIR)/phys
RNDR_DIR = $(SRC_DIR)/render
OBJ_DIR = build

# Files
C_FILES = $(wildcard   \
	$(SRC_DIR)/*.cpp   \
	$(CHAR_DIR)/*.cpp  \
	$(GUI_DIR)/*.cpp   \
	$(LEVEL_DIR)/*.cpp \
	$(TILE_DIR)/*.cpp  \
	$(MC_DIR)/*.cpp    \
	$(PART_DIR)/*.cpp  \
	$(PHYS_DIR)/*.cpp  \
	$(RNDR_DIR)/*.cpp  \
)
OBJ_FILES = $(C_FILES:.cpp=.o)

# Build
OBJ = $(OBJ_DIR)/$(notdir $@)
E_OBJ = $(foreach obj,$^,$(OBJ_DIR)/$(notdir $(obj)))

# Output executable
EXEC = Minecraft

# Rule to build the executable
$(EXEC): $(OBJ_FILES)
	$(CXX) -o $@ $(E_OBJ) $(CXXFLAGS)

# Rule to compile each .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJ)

# Clean rule to remove compiled files
clean:
	rm -f $(OBJ_FILES) $(EXEC)

init:
	$(MKDIR) $(OBJ_DIR)

.PHONY: clean init