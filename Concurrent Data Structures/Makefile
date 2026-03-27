CC      = clang
CFLAGS  = -Wall -Wextra -Werror -pedantic
LDLIBS  = ./asgn2_helper_funcs.a

TARGET  = queue.o rwlock.o
OBJS    = $(TARGET)

DEPS    = queue.h rwlock.h

all: queue.o rwlock.o 

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

rwlock.o: rwlock.c
	$(CC) $(CFLAGS) -c rwlock.c

clean:
	rm -f $(OBJS) 
	
format:
	clang-format -i -style=file *.[ch]