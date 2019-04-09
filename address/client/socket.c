

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../address.h"
#include "../login.h"
int InitNet()
{
	struct sockaddr_in server_addr;
	int sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		perror("error");
		exit(1);
	}
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=PF_INET;
	server_addr.sin_port=htons(11111);
	server_addr.sin_addr.s_addr=inet_addr("192.168.1.111");//nanopiaddress
	int ret=connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(-1==ret)
	{
		perror("connect");
		exit(1);
	}
	printf("success\n");
	return sockfd;
}
int  login_on(fd)
{
	acc c;
	int i;

	printf("请输入账号\n");
	scanf("%s",c.number);
/*	for(i = 0; i < 20; i++)
	{
		c.passward[i] = getch();
		if(c.passward[i] == '\r')
			break;
		putch('*');
	}
	c.passward[i] = '\n';*/
	strcpy(c.passward,getpass("请输入密码\n"));
	c.cmd=LOGINON; 
	int ret=send(fd,&c,sizeof(c),0);
	if(ret==-1)
	{
		perror("login_send");
		exit(1);
	}
	memset(&c,0,sizeof(c));
	ret=recv(fd,&c,sizeof(c),0);
	if(c.result == 1)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
void exit_login(int fd)
{
	acc c;
	c.cmd=LOGINOFF;
	int ret=send(fd,&c,sizeof(c),0);
	if(-1==ret)
	{
		perror("semd");
		exit(-1);
	}
	

}

int find_pass(int fd)
{
	acc c;
	c.cmd = UPDATE;
	printf("请输入你的账号\n");
	scanf("%s",c.number);
	printf("请输入密保问题\n");
	scanf("%s",c.question);
	printf("请输入密保答案\n");
	scanf("%s",c.answer);
	strcpy(c.passward,getpass("请输入修改后的密码\n"));

	int ret=send(fd,&c,sizeof(c),0);
	if(ret==-1)
	{
		perror("send");
	}
	
	char res;

	ret=recv(fd,&res,sizeof(res),0);
	if(res == 's')
	{
		printf("找回成功，请重新登录\n");
		return 1;
	}
	else
	{
		printf("找回失败\n");
		return 0;
	}
}

void register_info(int fd)
{
	acc c;
	printf("请输入账号\n");
	scanf("%s",c.number);
	printf("请输入密码\n");
	scanf("%s",c.passward);
	printf("请输入密保问题\n");
	scanf("%s",c.question);
	printf("请输入密保答案\n");
	scanf("%s",c.answer);
	c.cmd=REGISTER;
	int ret=send(fd,&c,sizeof(c),0);
	if(ret==-1)
	{
		perror("send");
	}
	
	char new;
	ret =recv(fd,&new,sizeof(new),0);
	if(new == 'f')
	{
		printf("创建失败\n");
	}
	else
	{
		printf("创建成功\n");
	}
}

void add_info(int fd)
{
	Chat c;
	printf("请输入名字和电话\n");
	scanf("%s%s",c.name,c.tel);
	printf("请输入要加入的群组\n");
	scanf("%s",c.group);
	c.cmd=ADDINFO;
	int ret=send(fd,&c,sizeof(c),0);
	if(-1==ret)
	{
		perror("send");
	}
	printf("add success!\n");
	sleep(1);
}

void exit_client(int fd)
{
	//tongzhi fuwuqi xiaxian
	//guanbi tcp lianjie
	//tichu chengxu
	Chat c;
	c.cmd = EXIT;
	int ret = send(fd,&c,sizeof(c),0);
	if(-1 == ret)
	{
		perror("error!\n");
	}
	close(fd);
	exit(0);
}

void show_info(int fd)
{
	Chat c;
	c.cmd = SHOWINFO;
	int ret = send(fd,&c,sizeof(c),0);
	if(-1 == ret)
	{
		perror("send");
	}

	int i = 0;
	while(1)
	{
		memset(&c,0,sizeof(c));
		ret = recv(fd,&c,sizeof(c),0);
		if(-1 == ret)
		{
			perror("recv");
		}
		if(c.result == 0)
		{
			printf("\033[44;30;7m%s:\033[0m\n",c.group);
		}
		else if(c.result == 1)
		{
			if(!strcmp(c.name,"bye")&&!strcmp(c.tel,"bye"))
			{
				break;
			}
			printf("name:%s   tele:%s\n",c.name,c.tel);
			i++;
		}
	}

	if(i == 0)
	{
		printf("通讯录为空\n");
	}
	sleep(1);
}

void delete_info(int fd)
{
	Chat c;
	int flag = 0;
	c.cmd = DELETEINFO;
	printf("请输入你要进入的组名\n");
	scanf("%s",c.group);
	printf("请输入你想要删除的名字\n");
	scanf("%s",c.name);
	int ret = send(fd,&c,sizeof(c),0);
	if(-1 == ret)
	{
		perror("send");
	}

	ret = recv(fd,&flag,sizeof(flag),0);
	if(-1 == ret)
	{
		perror("recv");
	}
	if(flag == 0)
	{
		printf("找不到这个名字\n");
	}
	else if(flag == 1)
	{
		printf("删除成功\n");
	}
	sleep(1);
}
void update_info(int fd)
{
	Chat c;
	int flag = 0;

	c.cmd=UPDATEINFO;
	printf("请输入你要进入的组名\n");
	scanf("%s",c.group);
	printf("请输入你想要更新的名字\n");
	scanf("%s",c.name);
	printf("请输入电话\n");
	scanf("%s",c.tel);
	printf("请输入您要更改的组别\n");
	scanf("%s",c.group);

	int ret = send(fd,&c,sizeof(c),0);
	if(ret == -1)
	{
		printf("send");
	}

	ret=recv(fd,&flag,sizeof(flag),0);
	if(ret == -1)
	{
		printf("recv");
	}

	if(flag == 0)
	{
		printf("找不到这个名字\n");
	}
	else if(flag == 1)
	{
		printf("修改成功\n");
	}
	sleep(1);
}

void research_info(int fd)
{
	Chat c;
	c.cmd = SEARCHINFO;
	printf("请输入要查找的组别\n");
	{
		scanf("%s",c.group);
	}
	printf("请输入你想要查找的名字\n");
	scanf("%s",c.name);

	int ret = send(fd,&c,sizeof(c),0);
	if(-1 == ret)
	{
		perror("send");
	}
	memset(&c,0,sizeof(c));
	ret=recv(fd,&c,sizeof(c),0);
	if(ret == -1)
	{
		perror("recv");
	}
	if(c.result == 0)
	{
		printf("找不到这个名字\n");
	}
	else
	{
		printf("你想要查找的信息：\n");		
		printf("name:%s    tele:%s   group:%s\n",c.name,c.tel,c.group);
	}
	sleep(1);
}

void create_group(int fd)
{
	Chat c;
	printf("请输入想创建的组名\n");
	scanf("%s",c.group);
	c.cmd=CREATEGROUP;

	int ret=send(fd,&c,sizeof(c),0);
	if(-1==ret)
	{
		perror("send");
	}

	printf("add success!\n");
	sleep(1);
}
