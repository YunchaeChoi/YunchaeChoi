#server:server.o
#	gcc -o server server.o
#client:client.o
#	gcc -o client client.o
#client.o:client.c
#	gcc -c -Wall -Werror client.c
#server.o:server.c
#	gcc -c -Wall -Werror server.c
all: server client

server:server.c
	gcc -o server server.c -Wall -Werror
client:client.c
	gcc -o client client.c -Wall -Werror
clean:
	rm server client
