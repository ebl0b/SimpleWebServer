#include "http_handle.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "utils.h"
#include "constants.h"

static const Route route_list[] = 	{{	".html", 	"text/html"	}, 
									{	".jpg", 	"image/jpeg"}};

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
	//printf("%s", buffer);
}

char* ProcessHTTP(char* buffer, int buffer_size, int* response_size){
	buffer[buffer_size] = '\0';
	char* response = NULL;
	char* body = NULL;
	int body_size = 0;
	int status_code = 0;
	char content_type[32];
	char method[128], uri[128];
	sscanf(buffer, "%s %s", method, uri);
	if((body = ReadFile(uri, &body_size)) != NULL){
		int i, route_list_size = sizeof(route_list);
		for(i = 0; strstr(uri, route_list[i].file_type)==NULL && i<route_list_size; i++);
		if(i==route_list_size){
			puts("Non supported file type");
			goto L404;
		}
		status_code = 200;
		strcpy(content_type, route_list[i].content_type);

	}else{
		L404:
		status_code = 404;
	}
	response = BuildHTTP(status_code, content_type, body, body_size, response_size);
	//printf("Full Response:\n%s\n", response);
	return response;
}

void SendHTTP(int socket_id, char *response, int response_size){
	int sent_bytes = 0;
	char* p = response;
	//printf("Full response size: %d\n", response_size);
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

