#ifndef ADDRESS_h
#define ADDRESS_H
#define ADDINFO 1000
#define DELETEINFO 1001
#define UPDATEINFO 1002
#define SEARCHINFO 1003
#define SHOWINFO   1004
#define EXIT       1005
#define CREATEGROUP 1006
struct ChatInfo
{
	int cmd;
	char name[32];
	char tel[32];
	char group[20];
	int result;//save hanhuizhi
};
typedef struct ChatInfo Chat;
#endif
