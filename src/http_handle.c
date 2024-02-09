#include "http_handle.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "utils.h"
#include "constants.h"

static Route route_list[] = 	{{"/index.html", "text/html"}, 
				{"/img/quokka.jpg", "image/jpeg"}, 
				{"END", "END"}};

static char* BuildHTTP(int status_code, char* content_type, char* body, int body_size, int* response_size){
	char* response = NULL;
	char status[32];
	char tmp[BUFFER_SIZE];
	if(status_code == 200){
		strcpy(status, "OK");
		sprintf(tmp, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", 
			status_code, status, body_size, content_type);
		int header_size = strlen(tmp);
		*response_size = header_size+body_size;
		response = calloc(*response_size, 1);
		strcpy(response, tmp);
		memcpy(response+header_size, body, body_size);
		free(body);
	}
	else{
		strcpy(status, "Not Found");
		char tmp2[BUFFER_SIZE];	
		sprintf(tmp, "HTTP/1.1 %d %s\r\n", status_code, status);
		*response_size = strlen(tmp)+1;
		response = calloc(*response_size, 1);
		strcpy(response, tmp);
	}
	return response;
}

void ReceiveHTTP(int socket_id, char* buffer, int buffer_size){
	memset(buffer, 0, buffer_size);
	if(recv(socket_id, buffer, buffer_size, 0) == -1){
		perror("Receive");
		exit(EXIT_FAILURE);
	}
	printf("%s", buffer);
}

char* ProcessHTTP(char* buffer, int buffer_size, int* response_size){
	buffer[buffer_size] = '\0';
	char* response = NULL;
	char* body = NULL;
	int body_size = 0;
	int status_code = 0;
	char content_type[32];
	char method[128], uri[128];
	perror("init done, in processing");
	sscanf(buffer, "%s %s", method, uri);
	perror("sscanf");
	printf("%d\n", strcmp(route_list[0].uri, uri));
	for(int i = 0;;i++){
		perror("in loop");
		if(strcmp(route_list[i].uri, uri) == 0){
			printf("test");
			body = ReadFile(uri, &body_size);
			printf("\nHTML Response:\n%s\n", body);	
			status_code = 200;
			strcpy(content_type, route_list[i].content_type);
			break;
		}
		else if(strcmp(route_list[i].uri, "END") == 0){
			status_code = 404;
			break;
		}
	}
	response = BuildHTTP(status_code, content_type, body, body_size, response_size);
	printf("Full Response:\n%s\n", response);
	return response;
}

void SendHTTP(int socket_id, char *response, int response_size){
	int sent_bytes = 0;
	char* p = response;
	printf("\n\ntotal response size: %d\n\n", response_size);
	while(response_size > 0){
		if((sent_bytes = send(socket_id, p, response_size, 0)) == -1){
			perror("Send");
			exit(EXIT_FAILURE);
		}
		p += sent_bytes;
		response_size -= sent_bytes;
	}
	free(response);
}

