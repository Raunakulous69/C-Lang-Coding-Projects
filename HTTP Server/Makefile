CC      = clang
CFLAGS  = -Wall -Wextra -Werror -pedantic
LDFLAGS =
LDLIBS  = ./asgn2_helper_funcs.a

TARGET  = httpserver
OBJS    = httpserver.o

DEPS    = debug.h iowrapper.h listener_socket.h protocol.h

all: $(TARGET)

$(TARGET): $(OBJS) asgn2_helper_funcs.a
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

httpserver.o: httpserver.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

format:
	clang-format -i -style=file *.[ch]