#include "acc.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef enum { false, true } bool;
struct stat sta = {0};


int countfile(char* dir)
{
	int counter = 0;
	DIR * pdir;
	struct dirent * dircheck;

	pdir = opendir(dir);
	
	while((dircheck = readdir(pdir)) != NULL)
	{
		if(dircheck->d_type == DT_REG)
		{
			counter++;
		}
	}
	closedir(pdir);
	return counter;
}

void controller(int sockfd, char *cAddressPort)
{
	printf("This is in control: %s\n", cAddressPort);
	char Make[MAXLINE] = "make", List[MAXLINE] = "list\n", Compose[MAXLINE] = "send", Read[MAXLINE] = "read", Delete[MAXLINE] = "delete", Quit[MAXLINE] = "quit\n", Err[MAXLINE] = "error\n", Ack[MAXLINE] = "done\n", Fin[MAXLINE] = "bye\n", Name[MAXLINE] = "name", Viewmail[MAXLINE] = "get_mailbox", DirErr[50] = "ERROR: No Directory";
	
	char line[MAXLINE], Temp[MAXLINE], logtime[MAXLINE], mailtime[MAXLINE], chopline[MAXLINE],  TempDir[MAXLINE];
	
	const char Directory[MAXLINE] = "/home/john-wick/Desktop/";

	char User[MAXLINE], recepient[MAXLINE];
	unsigned char buffer[MAXLINE]={0};

	char *token;
	char slash[2]="/", fileext[8]=".txt", delim[2]="-";
	
	char tempName[50], tempMessage[50], tempTime[50], mail_id[50]; 	
	bool firstclient;
	
	ssize_t	n;
	int mail_id_i, firsttime = 0, buffsize = 0, removeflag;	
	
	FILE *thismail;
	FILE *Mailinfo;	


	printf("loading socket %d...\n", sockfd);

	for ( ; ; ) 
	{

		printf("\nReading client ...\n");	
		n = Readline(sockfd, line, MAXLINE);

		
		
		strcpy(chopline, line);
		token = strtok(chopline, " ");
	
		Fputs(line, stdout);		

		//matching commands
		//match name command
		if(strcmp(Name, token) == 0)
		{
			token = strtok(NULL, " ");
			strcpy(User, token);
		
			Writen(sockfd, Ack, strlen(Ack));

			token = strtok(NULL, "\n");
			strcpy(logtime, token);			
				
			//check if there is directory
			if (stat(User, &sta) != -1)
			{
				mail_id_i = countfile(User);
			}
			else
			{
				mail_id_i = 0;
			}

			if(firsttime == 0)
			{
				//logs user to list
				setclientlist(User, logtime, cAddressPort, mail_id_i);
				firsttime++;
			}		
		}
		//match make command
		else if(strcmp(Make, token) == 0)
		{
			token = strtok(NULL, "\n"); //retrieve next argument
			strcpy(User, token); //store client name

			printf("making mailbox for ");//print path to server screen
			Fputs(User, stdout);
			
			if (stat(User, &sta) == -1)//check if the directory path available
			{
				mkdir(User, 0700);
				Writen(sockfd, Ack, strlen(Ack));
			}else
			{
				printf("Directory already exist\n");
			}
			
		}
		//match list command
		else if(strcmp(List, line) == 0)
		{
			printf("sending list of mail\n");
		}
		//match compose command
		else if(strcmp(Compose, token) == 0)
		{
			//plan: write to new write function do a mutex lock
			token = strtok(NULL, " "); //retrieve recepient name
			strcpy(recepient, token);
			
			if (stat(recepient, &sta) != -1)//check if there is a receiver mailbox
			{
				//there is a directory. proceed.
				mail_id_i = countfile(recepient);
				mail_id_i++;//file count increment for new file;

				sprintf(mail_id, "%d", mail_id_i); //convert file numbering
				
				//create file directory
				snprintf(TempDir, 50, "%s/%s%s", recepient, slash, mail_id, fileext);
				Fputs(TempDir, stdout);

				token = strtok(NULL, delim); //retrieve time argument
				strcpy(mailtime, token);				

				token = strtok(NULL, "\n"); //retrieve message argument

				//do a error check
				thismail = fopen(TempDir, "w+"); //create a file a write to it
				fprintf(thismail, " %s %s \r\n %s %s \r\n %s %s \n\n %s \n\n %s \n %s \n", "From:", User, "To:", recepient, "Date:", mailtime, token, "Regards", User); 
				fclose(thismail);

				Writen(sockfd, Ack, strlen(Ack));

				setmailinfo(mail_id_i, User, recepient, mailtime);			
			}else
			{
				printf("\nError, no receiver\n");
				Writen(sockfd, Err, strlen(Err));
			}
		}
		else if(strcmp(Viewmail, token) == 0)
		{
			token = strtok(NULL, "\n");
			strcpy(recepient, token);
			printf("TEST1\n");
			
			if (stat(recepient, &sta) != -1)//check if the directory path available
			{
				printf("TEST2\n");
				mail_id_i = countfile(recepient);
				
				char allmail[MAXLINE] = {'\0'};			

				getmailinfo(mail_id_i, recepient, allmail);			
				
				//write and read from log file
				Mailinfo = fopen("maillog.txt", "w+");				
				fprintf(Mailinfo, "%s", allmail);		
				fclose(Mailinfo);

				//send file in buffer of 1024
				Mailinfo = fopen("maillog.txt", "rb");
				buffsize = fread(buffer, 1, 1024, Mailinfo);
				fclose(Mailinfo);
				
				printf("TEST3 %s\n", allmail);
				printf("TEST3 %d\n", buffsize);			
				
				//check buffer size
				if(buffsize > 0 && buffsize < 1024)
				{
					printf("TEST4\n");
					Writen(sockfd, buffer, buffsize);
				}else
				{
					printf("ERROR: EXCEED BUFFER SIZE\n");	
				}
				
			}else
			{
				printf("\nERROR: no directory\n");				
				Writen(sockfd, Err, strlen(Err));
			}
		}
		//match read command
		else if(strcmp(Read, token) == 0)
		{
			token = strtok(NULL, " ");//retrieve client_name
			strcpy(recepient, token);

			token = strtok(NULL, "\n");
			strcpy(mail_id, token);			

			if (stat(recepient, &sta) != -1)//check if there is a receiver mailbox
			{
				//there is a directory. proceed.
				
				//create file directory
				snprintf(TempDir, 50, "%s/%s%s", recepient, slash, mail_id, fileext);
				Fputs(TempDir, stdout);

				//do a error check
				thismail = fopen(TempDir, "rb"); //create a file a write to it

				//send file in buffer of 1024
				buffsize = fread(buffer, 1, 1024, thismail);
				
				//check buffer size
				if(buffsize > 0 && buffsize < 1024)
				{
					Writen(sockfd, buffer, buffsize);
				}				
				fclose(thismail);
			}
			else
			{
				printf("\nError, no receiver\n");
				Writen(sockfd, Err, strlen(Err));
			}
		}
		//match delete command
		else if(strcmp(Delete, token) == 0)
		{
			token = strtok(NULL, " ");//retrieve client_name
			strcpy(recepient, token);

			token = strtok(NULL, "\n");
			strcpy(mail_id, token);
			
			if (stat(recepient, &sta) != -1)//check if there is a receiver mailbox
			{
				//there is a directory. proceed.
				
				//create file directory
				snprintf(TempDir, 50, "%s/%s%s", recepient, slash, mail_id, fileext);
				Fputs(TempDir, stdout);

				//remove the specified file
				removeflag = remove(TempDir);
				if(removeflag == 0)
				{
					printf("file removed successfully");
					Writen(sockfd, Ack, strlen(Ack));
				}
				else
				{
					printf("fail to remove");
					Writen(sockfd, Err, strlen(Err));
				}
			}
			else
			{
				printf("\nError, no receiver\n");
				Writen(sockfd, Err, strlen(Err));
			}

		}
		//match quit command
		else if(strcmp(Quit, line) == 0)
		{
			Writen(sockfd, Fin, strlen(Fin));
			return;
		}

		if ( n == 0)
		{
			return;		/* connection closed by client */
		}
		token = '\0';
	}
}
