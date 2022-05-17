all: mystat myls mytail myfind

CC=gcc

mystat: stat.c
	$(CC) -o $@ $<
myls: listFiles.c
	$(CC) -o $@ $<
mytail: tail.c
	$(CC) -o $@ $<
myfind: find.c
	$(CC) -o $@ $<

clean:
	rm mystat myls mytail myfind
