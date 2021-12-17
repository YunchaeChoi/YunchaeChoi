#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SIZE 255

int main(int argc, char* argv[])
{
	int n, serverfd, connfd, caddrlen;
	struct sockaddr_in saddr, caddr;
	char buffer[SIZE];
	int port = atoi(argv[1]);

	if((serverfd= socket(AF_INET,SOCK_STREAM,0)) < 0 )
	{
		printf("socket failed\n");
		exit(EXIT_FAILURE);
	}
	memset((char *)&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);
	if ( bind(serverfd, (struct sockaddr *)&saddr, sizeof(saddr)) <0)
	{
		printf("bind failed\n");
		exit(EXIT_FAILURE);
	}
	if( listen(serverfd,5) < 0)
	{
		printf("listen failed\n");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		caddrlen=sizeof(caddr);
		if((connfd = accept(serverfd,(struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) <0)
		{
			printf("accept failed.\n");
			continue;
		}
		while(( n=read(connfd,buffer,SIZE)) >0)
		{
			printf("got %d bytes from client\n",n);
		}
		printf("connection terminated\n");
		close(connfd);
	}
}
