

#include<stdio.h>
int main()
{
	int sockfd;
	sockfd=InitNet();
	InitLoginData();
	main_handler(sockfd);	
	return 0;
}
