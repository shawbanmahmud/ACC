#include "acc.h"

void *clithread(void *);

static int sockfd;		/* global for both threads to access */
static FILE *fp;


void clientprocess(FILE *fp_arg, int sockfd_arg)
{
	char recvline[MAXLINE];
	pthread_t tid;

	sockfd = sockfd_arg;	/* copy arguments to externals */
	fp = fp_arg;

	/* does no pass argument */
	Pthread_create(&tid, NULL, clithread, NULL);
	
	while(Readline(sockfd, recvline, MAXLINE) > 0)
	{
		sleep(1);
		Fputs(recvline, stdout);
		recvline[0] = '\0';
	}
	

}

void *clithread(void *arg)
{
	char receiveline[MAXLINE];
	char sendline[MAXLINE];
	
	char Name[MAXLINE] = "name";
	char Make[MAXLINE] = "make";
	char List[MAXLINE] = "list";	
	char Compose[MAXLINE] = "compose\n";
	char Send[MAXLINE] = "send";
	char View[MAXLINE] = "get_mailbox";
	char Read[MAXLINE] = "read";
	char Delete[MAXLINE] = "delete";
	
	char message[50];	
	char email[MAXLINE];
	
	char Help[MAXLINE] = "help\n";
	char Question[MAXLINE] = "?\n";
	char Quit[MAXLINE] = "quit\n";
	
	char chopline[MAXLINE];
	char *token, *timetoken;
	char delim[2] = "-";
	char temp[MAXLINE];

	int livestate = 1;
	int notok = 1;	

	Pthread_detach(pthread_self());
	
	do{
		printf("Please input your name to begin. USAGE: name <your_name>\n");
		printf("MES-C>");
		Fgets(sendline, MAXLINE, stdin);
		strcpy(chopline, sendline);	
		token = strtok(chopline, " ");
	
		if(strcmp(token, Name) == 0)
		{	
			strcpy(temp, token);

			token = strtok(NULL, "\n");
			if(token != '\0')
			{
				printf("Loading...\n");
				
				//time taken
				time_t tim;
				time(&tim);

				strcat(temp, " ");
				strcat(temp, token);		
				
				strcat(temp, " ");
				strcat(temp, ctime(&tim));				

				Writen(sockfd, temp, strlen(temp));
				notok = 0;
				sleep(5);
			}
			else
			{
				printf("ERROR: no names added\n");
			}
		}
		else if(strcmp(sendline, Quit) == 0)
		{
			printf("Loading...\n");
			Writen(sockfd, sendline, strlen(sendline));
			sleep(5);
		}
		else
		{
			printf("ERROR: not name command\n");
		}
	}while(notok);
	
	printf("\nHello %s. You have 'number' unread messages.\n", token);
	printf("For more commands, use <help> or <?>.");
	
	while (livestate)
	{	
		printf("\nMES-C>");
		Fgets(sendline, MAXLINE, stdin);
		
		strcpy(chopline, sendline);	
						
		token = strtok(chopline, " ");
		
		if(strcmp(token, Make) == 0)
		{
			token = strtok(NULL, "\n");
			if(token != '\0')
			{
				printf("Loading...\n");
				Writen(sockfd, sendline, strlen(sendline));	
				sleep(5);
			}
			else
			{
				printf("ERROR: invalid command usage\n");
			}
		}
		else if(strcmp(sendline, Compose) == 0)
		{
			printf("Write your message here:\n");
			if(Fgets(message, 50, stdin) == NULL)
			{	
				printf("\nERROR: no messages added\n");
			}
			else if(strlen(message) >= 50)
			{
				printf("\nERROR: maximum character limit reached\n");
			}
			else
			{
				printf("Message length: %d\n", strlen(message));
				printf("\nNOTE: Use 'send <receiver_name>' to send file.\n");
			}			
		}
		else if(strcmp(token, Send) == 0)
		{
			//time taken
			time_t tim;
			time(&tim);

			strcpy(temp, token); //send string
			
			token = strtok(NULL, "\n");
			if(token != '\0')
			{
				strcat(temp, " ");
				strcat(temp, token);
				strcat(temp, " ");

				token = strtok(ctime(&tim), "\n");
				strcat(temp, token);
				strcat(temp, delim);
				strcat(temp, message);
						
				printf("Sending...\n");
				Writen(sockfd, temp, strlen(temp));
				sleep(5);
			}
			else
			{
				printf("ERROR: invalid command usage\n");
			}
		}
		else if(strcmp(token, View) == 0 )
		{
			token = strtok(NULL, "\n");
			if(token != '\0')
			{
				printf("Loading...\n");
				Writen(sockfd, sendline, strlen(sendline));	
				sleep(10);
			}
			else
			{
				printf("ERROR: invalid command usage\n");
			}
		}
		else if(strcmp(token, Read) == 0)
		{
			token = strtok(NULL, "\n");
			if(token != '\0')
			{
				printf("Loading...\n");
				Writen(sockfd, sendline, strlen(sendline));	
				sleep(10);
			}
			else
			{
				printf("ERROR: invalid command usage\n");
			}
		}
		else if(strcmp(token, Delete) == 0)
		{
			token = strtok(NULL, "\n");
			if(token != '\0')
			{
				printf("Loading...\n");
				Writen(sockfd, sendline, strlen(sendline));	
				sleep(5);
			}
			else
			{
				printf("ERROR: invalid command usage\n");
			}
		}
		else if(strcmp(sendline, Help) == 0 || strcmp(sendline, Question) == 0)
		{
			printf("\n---------USAGE---------|---COMMAND---\n");
			printf("Create a mailbox         - make <client_name>\n");
			printf("Retrieve client list     - get_client_list\n");
			printf("Write message		 - compose\n");
			printf("Send message to receiver - send <receiver_name>\n");
			printf("View list of mails       - get_mailbox <client_name>\n");
			printf("Read mail in mailbox     - read <client_name> <mail_id>\n");
			printf("Delete mail in mailbox   - delete <client_name> <mail_id>\n");
			printf("Close program            - quit\n");
			printf("...end\n");
		}
		else if(strcmp(sendline, Quit) == 0)
		{
			livestate = 0;
			printf("Loading...\n");
			Writen(sockfd, sendline, strlen(sendline));
			sleep(5);
		}
		else
		{
			token = strtok(chopline, "\n");
			printf("Command '%s' not found, use <help> or <?>.\n", token);
		}
				
	}
	Shutdown(sockfd, SHUT_WR);	/* EOF on stdin, send FIN */
	pthread_exit(pthread_self());

	return(NULL);
}
