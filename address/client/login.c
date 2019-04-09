

#include<stdio.h>
#include "../login.h"
void login_menu()
{
	printf("\t\t\033[44;30;7m\t   登录通讯录   \t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   1.登录       \t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   2.注册       \t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   3.找回密码   \t\033[0m\n");
}
int login_handler(int fd)
{
	char choice[32]={0};
    char c = 'l';
	int ret=send(fd,&c,sizeof(c),0);
	if(ret == -1)
	{
		perror("senderror");
	}
	login_menu();
	scanf("%s",choice);
	ret = 0;
	switch(choice[0])
	{
		case '1':
			ret=login_on(fd);
			break;
		case '2':
			register_info(fd);
			break;
		case '3':
			find_pass(fd);
			break;
		default:
			printf("请选择正确的功能\n");
	}
	if(ret == 1)
	{
			exit_login(fd);
			return 1;
	}
	else 
	{
		exit_login(fd);
		printf("登录失败，程序退出\n");
		return 0;
	}
}
