

#include<stdio.h>


int main()
{
	int sockfd;
	sockfd=InitNet();//chushihuawangluo
	int ret=login_handler(sockfd);
	if(ret==1)
	{
		printf("登录成功!\n");
		sockfd=InitNet();
		main_handler(sockfd);
	}
	return 0;
}
