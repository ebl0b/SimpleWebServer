#include "http_handle.h"
#include "sock.h"
#include "utils.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>

void main(void){
	int listening_socket, serve_socket, addrlen, response_size;
	char buffer[BUFFER_SIZE];
	char* response = NULL;
	struct sockaddr_in address;
	addrlen = sizeof(address);

	listening_socket = CreateSocket();	
	puts("listening socket created...");
	BindSocket(listening_socket, &address, addrlen);
	puts("socket bound to port...");
	ListenToSocket(listening_socket, BACKLOG);
	puts("Server running...");
	for(;;){
		serve_socket = AcceptSocket(listening_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		ReceiveHTTP(serve_socket, buffer, BUFFER_SIZE);
		perror("Received");
		response = ProcessHTTP(buffer, BUFFER_SIZE, &response_size);
		perror("Processed");
		SendHTTP(serve_socket, response, response_size);
		CloseSocket(serve_socket);
	}	
}
