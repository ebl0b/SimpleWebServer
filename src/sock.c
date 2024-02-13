#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "constants.h"
#include "sock.h"
#include <stdio.h>
#include <errno.h>

int CreateSocket(void){
	int socket_id;
	if((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	return socket_id;
}

void BindSocket(int socket_id, struct sockaddr_in* address, int addr_size){
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);
	if(bind(socket_id, (struct sockaddr*)address, addr_size) < 0){
		perror("Bind");
		exit(EXIT_FAILURE);
	}
}

void ListenToSocket(int socket_id, int backlog_size){
	if(listen(socket_id, backlog_size) < 0){
		perror("Listen");
		exit(EXIT_FAILURE);
	}
}

int AcceptSocket(int socket_id, struct sockaddr* address, socklen_t* addr_size){
	int new_sock;
	if((new_sock = accept(socket_id, address, addr_size))<0){
		perror("Accept");
		exit(EXIT_FAILURE);
	}
	return new_sock;
}

void CloseSocket(int socket_id){
	close(socket_id);
}
