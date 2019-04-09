

#include<stdio.h>
#include "../login.h"
void menu()
{
	system("clear");
	printf("\n\t\t\033[44;30;7m\t       通讯录         \t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   1.增添     2.删除    \t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   3.修改     4.寻找    \t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   5.显示     6.新建分组\t\033[0m\n");
	printf("\t\t\033[47;30;1m\t   7.退出程序           \t\033[0m\n");
}

void main_handler(int fd)
{
	char choice[32] = {0};
	char c = 'a';
	int ret = send(fd,&c,sizeof(c),0);
	if(-1 == ret)
	{
		perror("main_send");
	}
	while(1)
	{
		menu();
		scanf("%s",choice);
		switch(choice[0])
		{
			case '1':
				add_info(fd);
				break;
			case '2':
				delete_info(fd);
				break;
			case '3':
				update_info(fd);
				break;
			case '4':
				research_info(fd);
				break;
			case '5':
				show_info(fd);
				break;
			case '6':
				create_group(fd);
				break;
			case '7':
				exit_client(fd);
				break;
			default:
				printf("input error\n");
		}
	}
}
