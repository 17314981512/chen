

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../address.h"
#include "../login.h"
int fd[1024]={0};
pthread_t tid;
int InitNet()
{
	struct sockaddr_in server_addr;
	int sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(-1==sockfd)
	{
		perror("socket");
	}
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=PF_INET;
	server_addr.sin_port=htons(11111);
	server_addr.sin_addr.s_addr=inet_addr("192.168.1.111");
	int ret=bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(-1==ret)
	{
		perror("bind");
		exit(1);
	}
	ret=listen(sockfd,10);
	if(-1==ret)
	{
		perror("listen");
		exit(1);
	}
	return sockfd;
}
void exit_client(int fd)
{
	printf("clien%d\n",fd);
	close(fd);
	pthread_exit((void *)0);
}
void *client_handler(void *arg)//xiancheng hanshu
//void client_handler(int fd)
{
	Chat c;
	int fd = *(int *)arg;
	while(1)
	{
		int ret = recv(fd,&c,sizeof(c),0);
		if(-1 == ret)
		{
			perror("recv");
		}
		switch(c.cmd)
		{
			case ADDINFO:
				add_info(&c);
				break;
			case DELETEINFO:
				delete_info(fd,&c);
				break;
			case UPDATEINFO:
				update_info(fd,&c);
				break;
			case SEARCHINFO:
				search_info(fd,&c);
				break;
			case SHOWINFO:
				show_info(fd);
				break;
			case CREATEGROUP:
				create_group(&c);
				break;
			case EXIT:
				exit_client(fd);
				break;
		}
	}
}
void *login_handler(void *arg)
{
	acc c;
	int fd = *(int *)arg;
	while(1)
	{
		int ret = recv(fd,&c,sizeof(c),0);
		if(-1 == ret)
		{
			perror("recv");
		}
		switch(c.cmd)
		{
			case LOGINON:
				loginon(fd,&c);
				break;
			case REGISTER:
				register_info(fd,&c);
				break;
			case LOGINOFF:
				exit_login(fd);
				break;
			case UPDATE:
				find_pass(fd,&c);
				break;
		}
	}
}
void main_handler(int sockfd)
{
	struct sockaddr_in client_addr;
	int i=0;
	char c;
	int length=sizeof(client_addr);
	while(1)
	{
		for(i=0;i<1024;i++)
		{
			if(0==fd[i])
			{
				break;
			}
		}
		fd[i]=accept(sockfd,(struct sockaddr *)&client_addr,&length);
		printf("receive the connection: %d\n",fd[i]);
		int ret=recv(fd[i],&c,sizeof(c),0);
		if(-1==ret)
		{
			perror("recv");
		}
		if(c=='l')
		{
			ret=pthread_create(&tid,NULL,login_handler,&fd[i]);
				if(-1==ret)
			{
				perror("pthread_create");
				exit(1);
			}

		}
		else if(c=='a')
		{
			ret=pthread_create(&tid,NULL,client_handler,&fd[i]);
			if(-1==ret)
			{
				perror("pthread_create");
				exit(1);
			}
		}
	}
}

