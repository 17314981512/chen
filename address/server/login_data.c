#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string.h>
#include"../address.h"
#include "../login.h"


int flag = 0;  //标记查询结果
char tmp[128] = {0};//存放用户名

void InitLoginData()
{
	sqlite3 *db;
	int ret=sqlite3_open("account.db",&db);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
		exit(-1);
	}
	char sql[128]={0};
	sprintf(sql,"create table if not exists account(number text primary key,passward text);");
	ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_exec:%s",sqlite3_errmsg(db));
		exit(1);
	}
	sprintf(sql,"create table if not exists passward(number text,question text,answer text);");
	ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_exec:%s",sqlite3_errmsg(db));
		exit(1);
	}
	sqlite3_close(db);
}
int login_back(void *para,int columnCount,char **columnValue,char **columnName)
{
	int fd=*(int *)para;
	sqlite3 *db;
	int ret=sqlite3_open("address.db",&db);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_open:%s",sqlite3_errmsg(db));
		exit(-1);
	}
	if(columnValue[0]!=NULL&&columnValue[1]!=NULL)
	{
		flag=1;
		char sql[128] = {0};
		sprintf(sql,"create table if not exists '%s'(name text primary key,tel text,group text);",columnValue[0]);
		strcpy(tmp,columnValue[0]);
		ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
	}
	sqlite3_close(db);
	return 0;
}
void loginon(int fd,acc *c)
{
	sqlite3 *db;
	int ret=sqlite3_open("account.db",&db);
	flag = 0;
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_open:%s",sqlite3_errmsg(db));
		exit(-1);
	}
	char sql[128]={0};
	sprintf(sql,"select * from account where number='%s' and passward='%s';",c->number,c->passward);
	ret=sqlite3_exec(db,sql,login_back,&fd,NULL);
	acc new;
	if(flag==1)
	{
		new.result=1;
	}
	else if(flag==0)
	{
		new.result=0;
	}
	strcpy(new.number,c -> passward);
	ret=send(fd,&new,sizeof(new),0);
	if(-1==ret)
	{
		perror("send");
	}
	sqlite3_close(db);
}
void exit_login(int fd)
{
	close(fd);
	pthread_exit((void *)0);
}
void register_info(int fd,acc *c)
{
	sqlite3 *db;
	int ret=sqlite3_open("account.db",&db);
	if(ret!=SQLITE_OK);
	{
		printf("sqlite3_open file:%s",sqlite3_errmsg(db));
	}
	char sql[128]={0};
	flag = 0;

	sprintf(sql,"insert into account values('%s','%s');",c->number,c->passward);
	ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_exec:%s",sqlite3_errmsg(db));
		flag = 1;
	}	
	
	sprintf(sql,"insert into passward values('%s','%s','%s');",c->number,c->question,c->answer);
	ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_exec:%s",sqlite3_errmsg(db));
		flag = 1;
	}

	char new;

	if(flag = 1 )
	{
		new = 'f';
	}
	else
	{
		new = 's';
	}

	ret=send(fd,&new,sizeof(new),0);
	if(-1==ret)
	{
		perror("send");
	}
	sqlite3_close(db);
}

int find_back(void *para,int columnCount,char **columnValue,char **columnName)
{
	int fd=*(int *)para;
	acc c;
	if(columnValue[0]!=NULL&&columnValue[1]!=NULL)
	{
		flag=1;
	}
	return 0;
}

void find_pass(int fd,acc *c)
{
	sqlite3 *db;
	int ret=sqlite3_open("account.db",&db);
	if(ret!=SQLITE_OK);
	{
		printf("sqlite3_open file:%s",sqlite3_errmsg(db));
	}

	char sql[128]={0};
	sprintf(sql,"select * from passward where number='%s' and question='%s' and answer='%s';",c->number,c->question,c->answer);
	ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_exec:%s",sqlite3_errmsg(db));
	}

	flag = 0;
	ret=sqlite3_exec(db,sql,find_back,&fd,NULL);
	char res;

	if(flag == 1)
	{
		sprintf(sql,"update account set passward='%s' where number='%s';",c->passward,c->number);
		ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
		if(ret!=SQLITE_OK)
		{
			printf("sqlite3_exec:%s",sqlite3_errmsg(db));
		}
		res = 's';
	}
	else
	{
		res = 'f';
	}
	ret=send(fd,&res,sizeof(res),0);
	if(-1==ret)
	{
		perror("send");
	}
	sqlite3_close(db);
}
