CC = gcc
TARGET = Virtual_Memory_Manager
OBJS = virtual_memory_manager.o

CFLAGS = -Wall -Werror

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(TARGET) $(OBJS)
