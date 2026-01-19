CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11

TARGET  = allocator_test

SRCS    = allocator.c block.c test.c
OBJS    = $(SRCS:.c=.o)
OBJS    := $(addprefix obj/, $(OBJS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

obj/%.o: %.c allocator.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf obj $(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild
