#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define SIZE sizeof(struct sockaddr_in)

void closesock(int sig);

int sockfd_connect;

int main()
{
    int sockfd_listen;
    char c;
    struct sockaddr_in server = {AF_INET, 5000, INADDR_ANY};
    struct sigaction act;

    act.sa_handler = closesock;
    sigfillset(&(act.sa_mask));
    sigaction(SIGPIPE,&act,NULL);

    if((sockfd_listen=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("fail to call socket\n");
        exit(1);
    }

    if(bind(sockfd_listen,(struct sockaddr *)&server, SIZE)==-1)
    {
        printf("fail to call bind\n");

        exit(EXIT_FAILURE);
    }

    if(listen(sockfd_listen,5)==-1)
    {
        printf("fail to call listen\n");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        if((sockfd_connect = accept(sockfd_listen,NULL, NULL))==-1)
        {
            printf("fail to call accept\n");
            continue;
        }
        printf("accepted\n");
        while(recv(sockfd_connect,&c,1,0) > 0)
            send(sockfd_connect,&c,1,0);

        printf("close(sockfd_connect)\n");
        close(sockfd_connect);
    }

}

void closesock(int sig)
{
    close(sockfd_connect);
    printf("connection is lost\n");
    exit(0);
=======
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
>>>>>>> bba8f616135ad10d14f72ba88634709a05b397d6
}
