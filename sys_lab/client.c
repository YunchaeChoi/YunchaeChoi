#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>

#define SIZE sizeof(struct sockaddr_in)

int main()
{
    int errnum;
    int sockfd;
    char send_c, recv_c;
    struct sockaddr_in server = {AF_INET,5000};

    server.sin_addr.s_addr  = inet_addr("127.0.0.1");

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("fail to call socket\n");
        exit(1);
    }


    int temp=connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    printf("%d\n",temp);

    if(temp==-1)
    {
        printf("fail to call connect\n");
        //exit(1);
    }
    errnum=errno;
    printf("value of error: %d\n",errno);
    exit(1);

    recv_c = '\n';
    while(1)
    {
        if(recv_c == '\n')
            printf("Input a message\n");
        send_c = getchar();

        send(sockfd,&send_c,1,0);

        if(recv(sockfd, &recv_c,1,0) >0)
            printf("%c",recv_c);
        else
        {
            printf("server has no reply\n");
            close(sockfd);
            exit(1);
        }
    }
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
	char buffer[SIZE];
	int clientfd; // client socketfd 
	const char* host= argv[1];
	struct hostent *h;
	struct sockaddr_in saddr;
	int port = atoi(argv[2]);

	if((clientfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("socket failed\n");
		exit(EXIT_FAILURE);
	}

	if((h = gethostbyname(host)) == NULL)
	{
		printf("invalid host name %s\n",host);
		exit(EXIT_FAILURE);
	}

	memset((char*)&saddr, 0, sizeof(saddr));
	saddr.sin_family=AF_INET;
	memcpy((char*)&saddr.sin_addr.s_addr, (char*)h->h_addr, h->h_length);
	saddr.sin_port=htons(port);

	if(connect(clientfd,(struct sockaddr *)&saddr, sizeof(saddr)) < 0) 
	{ 
		printf("connect() failed.\n");
		exit(3);
	}
	
	char* file_name=NULL;
	char* line=NULL;
	size_t linecap;
	ssize_t linelen;
	linelen=getline(&file_name,&linecap,stdin);
	printf("Client: File name is %s.\n",file_name);
	char path[30];
	sprintf(path,"./%s",file_name);
	printf("path: %s\n",path);
	path[strlen(path)-1]='\0';
	FILE* fp=fopen(path,"r");
	if(fp==NULL) printf("fp error\n");
	while(getline(&line,&linecap,fp) != -1)
	{
		printf("line: %s\n",line);
		strcpy(buffer,line);
		printf("buffer: %s\n",buffer);
		char* newline = strchr(buffer,'\n');
		if(newline)
		{
			printf("newline character detected\n");
			*newline='\0';
		}	
		write(clientfd,buffer,sizeof(buffer));
	}

>>>>>>> bba8f616135ad10d14f72ba88634709a05b397d6
}
