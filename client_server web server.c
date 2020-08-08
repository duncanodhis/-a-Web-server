#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <netdb.h>
#include <sys/sendfile.h>
char webpage[]= 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html;charset=UTF-8\r\n\r\n"
"<DOCTYPE html>\r\n"
"<html><head><title>SHEllWaves</title>\r\n"
"<style>body{background-color:#FFFF00}</style></head>\r\n"
"<body><center><h1>Hellow world</h1><br></center></body></html>\r\n\r\n";
//the website

int main(int argc ,char *argv[])
{
	struct sockaddr_in server_addr,client_addr;
	socklen_t sin_len=sizeof(client_addr);
	int fd_server,fd_client;
	char buf[2018];
	int fding;
	int on=1;
	char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
	
	fd_server=socket(AF_INET,SOCK_STREAM,0);//create an endpoint for communication & return  file descriptor
	//listening to incoming connections
	if(fd_server<0){
		perror("socket");
		exit(1);
		
	}
	
	setsockopt(fd_server,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(int));
	server_addr.sin_family=AF_INET;//match the socket() call
    server_addr.sin_addr.s_addr=INADDR_ANY;//bind to any local address	
    server_addr.sin_port=htons(8000);//specify port to listen on
    //
    if(bind(fd_server,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1){//bind
    	perror("bind");
    	close(fd_server);
    	exit(1);
	}
	
	
	if(listen(fd_server,10)==-1){//listen for incoming connection
			perror("listen");
        	close(fd_server);
        	exit(1);
	}
	
	while(1)
	{
		fd_client=accept(fd_server,(struct sockaddr *)&client_addr,&sin_len);//Accept one
		if(fd_client==-1){
			perror("connection failed...\n");
			continue;
		}
		printf("got client connection...\n");
		if(!fork()){
				//child process
				close(fd_server);
				
				memset(buf,0,2048);
				
				read(fd_client,buf,2047);
				printf("%s\n",buf);
				write(fd_client,webpage,sizeof(webpage)-1);
				send(fd_client, httpHeader, sizeof(httpHeader), 0);
				close(fd_client);
				printf("closing...\n");
				exit(0);
		}
		close(fd_client);//parent processs
	}
	
	return 0;
}

