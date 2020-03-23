#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

/* Client's settings on server */
typedef struct {
	struct sockaddr_in addr;	/* Client's remote address */
	int clientfd;			/* Client's connection fd */
	int uid;			/* Client's user ID */
	char name[32];			/* Client's nickname */
	int authorized;			/* 1 - yes, 0 - no */
} client_t;
