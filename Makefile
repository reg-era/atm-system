# Compiler and flags
CC = cc
CFLAGS = -Wall -g -I./include  # Add include directory for header files

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = bin/obj
DATA_DIR = data

# Files
EXEC = atm
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/system.o $(OBJ_DIR)/auth.o

# Targets

# Default target
all: $(BIN_DIR)/$(EXEC)

# Linking the objects to create the final executable in the bin/ directory
$(BIN_DIR)/$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Compile .c files into .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: all
	./$(BIN_DIR)/$(EXEC)

# Clean up the build
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(EXEC)

# Rebuild the project
rebuild: clean all

# Prevent make from getting confused by files named 'clean', 'all', etc.
.PHONY: all clean run rebuild
