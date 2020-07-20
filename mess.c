#include "acc.h"

void thread_creation(void *);
static void *thread_process(void *);

char cAddressPort[50], temp[50];

int main(int argc, char **argv)
{
	int listen_fd, *connfd, error, so;
	socklen_t addrlen, alen, adlen;
	struct sockaddr_in address;	
	char stradd[100];
	
	adlen = sizeof(address);

	memset(&address, 0, adlen);
	
	//listen for incoming connections
	if(argc==2)
	{
		listen_fd = Tcp_listen(NULL, argv[1], &addrlen);
	}
	else if(argc==3)
	{
		listen_fd = Tcp_listen(argv[1], argv[2], &addrlen);
	}
	else
	{
		err_quit("usage: server [host] [port]");
	}

	printf("Server Initializing..");	
		
	for(; ;)
	{
		alen = addrlen;
		connfd = Malloc(sizeof(int));
		
		memset(&address, 0, adlen);

		//Accept client connection
		*connfd = Accept(listen_fd, address, &adlen);

	
		int erro = getpeername(*connfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if(erro != 0)
		{
			printf("ERROR: getpeername\n");
			//exit(1);
		}
		
		sprintf(temp, "%d", ntohs(address.sin_port));
		snprintf(cAddressPort, 100, "%s:%s", inet_ntoa(address.sin_addr), temp);	
		
		printf("Connection info with %s\n", cAddressPort);
	
		pthread_t tid;
		
		//create thread for each client
		error = pthread_create(&tid, NULL, thread_process, connfd);
		if(error != 0)
		{
			printf("pthread creation fail");
			Close(connfd);
		}
	}
}

static void *thread_process(void *arg)
{
	int connfd;
	printf("Created this thread: %lu \n", pthread_self());
	sleep(5);
	connfd = *((int*)arg);
	free(arg);
	pthread_detach(pthread_self()); //Detach from parents
	controller(connfd, cAddressPort); //Handling the client
	Close(connfd);

	pthread_exit(pthread_self());
	exit(0);
}
