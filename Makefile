CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = main.c
TARGET = ray_pong

# Auto-detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lraylib
else
    # Fallback for Windows
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
endif

.PHONY: all build run clean

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)