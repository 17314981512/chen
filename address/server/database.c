

#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string.h>
#include"../address.h"
#include "../login.h"

int flag;
extern char tmp[128];

void add_info(Chat *c)
{
	sqlite3 *db;
	int ret = sqlite3_open("address.db",&db);
	if(ret!=SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
	}

	char sql[128] = {0};
	 

    sprintf(sql,"insert into '%s' values ('%s','%s','%s');",tmp,c -> name,c -> tel, c -> group);
	ret = sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec:%s\n",sqlite3_errmsg(db));
		exit(1);
	}

	sqlite3_close(db);
}

int  send_info(void *para,int columnCount,char **columnValue,char **columnName)
{
	int fd = *(int *)para;
	Chat c;

	strcpy(c.name,columnValue[0]);
	strcpy(c.tel,columnValue[1]);

	c.result = 1;
	int ret = send(fd,&c,sizeof(c),0);
	if(ret == -1)
	{
		perror("send");
	}

	return 0;
}

int table_info(void *para,int columnCount,char **columnValue,char ** columnName)
{	
	sqlite3 *db;
	int	ret = sqlite3_open("address.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
	}

	int fd = *(int *)para;
	Chat c;
	char sql[128]={0};
	sprintf(sql,"select * from '%s' where group = '%s';",tmp,columnValue[0]);
	strcpy(c.group,columnValue[1]);

	c.result = 0;
	ret = send(fd,&c,sizeof(c),0);
	if(ret == -1)
	{
		perror("send");
	}

	ret = sqlite3_exec(db,sql,send_info,&fd,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec:%s\n",sqlite3_errmsg(db));
		exit(1);
	}
	return 0;
}

void show_info(int fd)
{
	sqlite3 *ppdb;
	int ret = sqlite3_open("address.db",&ppdb);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(ppdb));
	}

	char sql[128]={0};
	sprintf(sql,"select group from '%s';",tmp);
	ret = sqlite3_exec(ppdb,sql,table_info,&fd,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec:%s\n",sqlite3_errmsg(ppdb));
		exit(1);
	}

	Chat c;
	c.result = 1;
	strcpy(c.name,"bye");
	strcpy(c.tel,"bye");
	ret = send(fd,&c,sizeof(c),0);
	if(-1 == ret)
	{
		perror("send");
	}

	sqlite3_close(ppdb);
}


void delete_info(int fd,Chat * c)
{
	sqlite3 *db;
	int ret = sqlite3_open("address.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
		exit(-1);
	}

	char sql[128] = {0};
	sprintf(sql,"delete from '%s' where name='%s';",tmp,c->name);
	ret = sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(-1 == ret)
	{
		perror("delete");
	}
	if(sqlite3_changes(db) == 1)
	{
		flag = 1;
	}
	else
	{
		flag = 0;
	}
	ret = send(fd,&flag,sizeof(flag),0);
	if(-1 == ret)
	{
		perror("send");
	}

	sqlite3_close(db);
}


void update_info(int fd,Chat *c)
{
	sqlite3 *db;
	int ret = sqlite3_open("address.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
		exit(-1);
	}

	char sql[128]={0};
	sprintf(sql,"update '%s' set tel='%s'and group = '%s' where name='%s';",tmp,c->tel,c->group,c->name);
    ret = sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(sqlite3_changes(db) == 1)
	{
		flag = 1;
	}
	else
	{
		flag = 0;
	}
	ret = send(fd,&flag,sizeof(flag),0);
	if(-1 == ret)
	{
		perror("send");
	}

	sqlite3_close(db);
}


int  search_back(void *para,int columnCount,char **columnValue,char **columnName)
{
	int fd = *(int *)para;
	Chat c;
	strcpy(c.name,columnValue[0]);
	strcpy(c.tel,columnValue[1]);
	strcpy(c.group,columnValue[2]);
	c.result = 1;
	int ret = send(fd,&c,sizeof(c),0);
	if(ret == -1)
	{
		perror("send");
	}

	flag = 1;
	return 0;
}
void search_info(int fd,Chat *c)
{
	sqlite3 *db;
	int ret = sqlite3_open("address.db",&db);
	flag = 0;
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
		exit(-1);
	}

	char sql[128] = {0};
	sprintf(sql,"select * from '%s' where name='%s';",tmp,c->name);
	ret = sqlite3_exec(db,sql,search_back,&fd,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
		exit(-1);
	}	

	if(flag == 0)
	{
		c->result = 0;
		ret=send(fd,&c,sizeof(c),0);
	}

	sqlite3_close(db);
}


void create_group(Chat *c)
{
	sqlite3 *db;
	int ret = sqlite3_open("address.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(db));
	}

	char sql[128] = {0};
    sprintf(sql,"create table if not exists '%s' (name text primary key,tel text);",c->group);
	ret = sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec:%s\n",sqlite3_errmsg(db));
		exit(1);
	}

	sqlite3_close(db);
}
