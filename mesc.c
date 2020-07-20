#include "acc.h"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 3)
	{
		err_quit("usage: tcpcli <IPaddress> <port number>");
	}

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	printf("Welcome to MES. Your trusted mail provider.\n");

	clientprocess(stdin, sockfd); //Handles client user input
	close(sockfd);
	exit(0);
}
