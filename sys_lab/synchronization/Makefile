CFLAGS = -Wall -Werror
TARGET = synchronization
OBJS = monte_carlo.o
CC = gcc

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ -pthread -lm
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm $(TARGET) $(OBJS)
