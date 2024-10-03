CC = gcc
CFLAGS = -g -Iinclude -lglfw3 -lz -lopengl32 -lgdi32 -lglu32 -m64

# Directories
SRC_DIR = src
JAVA_DIR = $(SRC_DIR)/java
LEVEL_DIR = $(SRC_DIR)/level
PHYS_DIR = $(SRC_DIR)/phys
OBJ_DIR = build

# Files
C_FILES = $(wildcard $(SRC_DIR)/*.c $(JAVA_DIR)/*.c $(LEVEL_DIR)/*.c $(PHYS_DIR)/*.c)
OBJ_FILES = $(C_FILES:.c=.o)

# Build
OBJ = $(OBJ_DIR)/$(notdir $@)
E_OBJ = $(foreach obj,$^,$(OBJ_DIR)/$(notdir $(obj)))

# Output executable
EXEC = Minecraft

# Rule to build the executable
$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $(E_OBJ) $(CFLAGS)

# Rule to compile each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJ_DIR)/$(notdir $@)

# Clean rule to remove compiled files
clean:
	rm -f $(OBJ_FILES) $(EXEC)

.PHONY: clean