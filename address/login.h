#ifndef LOGIN_H
#define LOGIN_h
#define LOGINON 1010
#define LOGINOFF 1011
#define REGISTER 1012
#define UPDATE  1013
struct Accout
{
	int cmd;
	char number[20];
	char passward[20];
	char question[50];
	char answer[20];
	int result;
};
typedef struct Accout acc;
#endif

