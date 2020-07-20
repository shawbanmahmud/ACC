#include "acc.h"
typedef enum { false, true } bool;

static int clientcount = 0;

const int structcount = 10;

void setNclient(int);
void setmailinfo(int, char*, char*, char*);
void setclientlist(char*, char*, char*, int);
char* getmailinfo(int, char*, char*);

struct mailbox_info
{
	int email_id;
	char sender_name[20];
	char time[50];
	bool status; //read
};

struct client_list
{
	char client_name[50];
	char timejoin[50];
	char clientIP_Port[50];
	int totalmail;
	struct mailbox_info info[structcount];	
};

static struct client_list clist[structcount];

//determine number of client structs(For future dynamic
void clicount(bool countnow)
{
	if(countnow)
	{
		clientcount++;
	}else
	{
		clientcount--;
	}
}


//initialize number of client_list
void setNclient(int input)
{
	printf("setNclient test1%d\n", input);

}

//this is setting the mail struct content
void setmailinfo(int filecount, char *sender, char *owner, char *intime) //sender, owner is recepient
{
	//loop client list to match owner
	int i;
	for(i=1; i<structcount; i++)
	{
		//compare owner name with clist name
		if(strcmp(clist[i].client_name, owner) == 0)
		{
			clist[i].info[filecount].email_id = filecount;
			strcpy(clist[i].info[filecount].sender_name, sender);
			strcpy(clist[i].info[filecount].time, intime);
			clist[i].info[filecount].status = false;
		}
	}
	printf("clientlist test1: %s - %d -%s\n", clist[1].client_name, clist[1].totalmail, clist[1].timejoin);
	printf("clientlist test2: %s - %d -%s\n", clist[2].client_name, clist[2].totalmail, clist[2].timejoin);
	printf("clientlist test3: %s - %d -%s\n", clist[3].client_name, 	clist[3].totalmail, clist[2].timejoin);
	printf("setmailinfo test1: %s\n", clist[1].info[filecount].time);
}

//increment clientcount and add owner to clist
void setclientlist(char *owner, char *intime, char *oaddr, int nmail)
{
	clientcount++;	
	strcpy(clist[clientcount].client_name, owner);
	strcpy(clist[clientcount].timejoin, intime);
	strcpy(clist[clientcount].clientIP_Port, oaddr);
	clist[clientcount].totalmail = nmail;
}

char* getmailinfo(int filecount, char *owner, char* allmailinfo)
{
	char Temp[MAXLINE];
	int i, k;
	for(i=1; i<=clientcount; i++)
	{
		printf("TEST1.1\n");
		if(strcmp(clist[i].client_name, owner)==0)
		{
			printf("TEST1.2\n");
			
			if(filecount != 0)
			{			
				for(k=1; k<=filecount; k++)
				{
					printf("TEST1.3\n");
					snprintf(Temp, MAXLINE, "email_id:%d...sender_name:%s ...time:%s ...read:%s\n", clist[i].info[k].email_id, clist[i].info[k].sender_name, clist[i].info[k].time), clist[i].info[k].status ? "read" : "unread";
					printf("TEST1.4\n");
					strcat(allmailinfo, Temp);
					printf("TEST1.5\n");
				}
			}else
			{
				strcpy(allmailinfo, "No mail\n");
			}
		}
	}
	printf("TEST1.6: %s\n", allmailinfo);
	return allmailinfo;
}


