# Makefile for the Advanced Inventory Management System

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = inventory_app

# Object files to build
OBJS = main.o inventory.o item.o file_io.o utils.o

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compiling object files
main.o: main.c inventory.h file_io.h utils.h
	$(CC) $(CFLAGS) -c main.c

inventory.o: inventory.c inventory.h item.h
	$(CC) $(CFLAGS) -c inventory.c

item.o: item.c item.h
	$(CC) $(CFLAGS) -c item.c

file_io.o: file_io.c file_io.h inventory.h
	$(CC) $(CFLAGS) -c file_io.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Reset database (useful for testing)
clean_db:
	rm -f inventory_data.txt