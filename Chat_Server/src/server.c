#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <client_t.h>
#include <session.h>

void handler_SIGINT(int sig){
	char c;
	signal(sig,SIG_IGN);
	printf("\nDo you really want to stop the chatroom? [y/n] ");
	c = getchar();
	if(c == 'y' || c == 'Y'){
		exit(0);
	}
	else
		signal(SIGINT, handler_SIGINT);
	getchar();
}

void handler_SIGTSTP(int sig){
	char c;
	signal(sig,SIG_IGN);
	printf("\nDo you really want to stop the chatroom? [y/n] ");
	c = getchar();
	if(c == 'y' || c == 'Y'){
		exit(0);
	}
	else
		signal(SIGTSTP, handler_SIGTSTP);
	getchar();
}

int main(int argc, char *argv[])
{
	int port; 
	int listenfd; 			
	int connfd; 							
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pthread_t thread; 						/* Thread for new user */
	client_t* client; 						/* New client */
	size_t uid = 0; 						/* User's personal ID on this session */
	client_t* clients[MAX_CLIENTS] = {NULL};			/* Clients List */
	session_args_t session_args; 					/* Arguments for session function */
	socklen_t clilen = sizeof(client_addr);
	
	/* Usage: ./server port_number */
	if (argc != 2)
	{
        	printf("Usage %s <port>\n", argv[0]);
        	return EXIT_FAILURE;
    	}
	
	/* Socket settings */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
		perror("Error socket().");
		return EXIT_FAILURE;
	}
	memset((char *) &server_addr, 0, sizeof(server_addr));
    	port = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
    	server_addr.sin_addr.s_addr = INADDR_ANY;
    	server_addr.sin_port = htons(port);
	
	/* Binding */
	if (bind(listenfd, (struct sockaddr *) &server_addr,
        sizeof(server_addr)) < 0)
	{
		perror("Error bind().");
		return EXIT_FAILURE;
	}
		
	/* Listening */	
	if (listen(listenfd,5) < 0)
	{
		perror("Error listen().");
		return EXIT_FAILURE;
	}
	
	puts("------------CHATROOM READY------------");
	
	/* First argument for all sessions */
	session_args.clients = clients;
	
	signal(SIGINT, handler_SIGINT);
	signal(SIGTSTP, handler_SIGTSTP);
	/* Accept clients */
	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr *) &client_addr, &clilen);
		if (connfd < 0)
		{
			perror("Error accept().");
			continue;
		}
		
		/* Check if MAX_CLIENTS is reached */
		if ((get_client_count() + 1) == MAX_CLIENTS){
			printf("[SYSTEM]: Max clients is reached!");
			close(connfd);
			continue;
		}
		else
		{
			uid++;
		}
		
		/* Set up client's settings */
		client = (client_t*)malloc(sizeof(client_t));
		client->addr = client_addr;
		client->clientfd = connfd;
		client->uid = uid;
		client->authorized = 0;
		sprintf(client->name, "guest%lu", uid);
		
		/* Add user to UserList and create new thread */
		add_user(client, clients);
		
		/* Second argument for this user session */
		session_args.client = client;
		
		/* Create new thread */
		if(pthread_create(&thread, NULL, &session, (void*) &session_args))
		{
			close(connfd);
			delete_user(client->uid, clients);
			free(client);
		}
	}
	return 0;
}
