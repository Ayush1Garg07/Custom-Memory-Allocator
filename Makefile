CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -Iinclude

TARGET = allocator_test

SRC = src/allocator.c src/block.c src/test.c
OBJ = $(SRC:src/%.c=obj/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

obj/%.o: src/%.c include/allocator.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
