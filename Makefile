# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# --- OS-SPECIFIC LINKER FLAGS ---
# DEFAULT: Linux Linker Flags
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# WINDOWS (MinGW): Uncomment the line below and comment out the Linux line above
# LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

# MACOS: Uncomment the line below and comment out the Linux line above
# LDFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lraylib
# ---------------------------------

# Source files and output name
SRC = main.c
TARGET = ray_pong

# Default target
all: build run

# Compile the game
build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Run the game
run:
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)