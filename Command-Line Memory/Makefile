CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic

all : memory

memory: memory.o
	$(CC) -o memory memory.o

memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c

clean:
	rm -f memory.o memory

format: 
	clang-format -i -style=file *.[ch]

