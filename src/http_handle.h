

void	ReceiveHTTP	(int socket_id, char* buffer, int buffer_size);
char*	ProcessHTTP	(char* buffer, int buffer_size, int* response_size);
void	SendHTTP	(int socket_id, char* response, int response_size);

#ifndef HTTP_HANDLE
#define HTTP_HANDLE

typedef struct{
	char		file_type[32];
	char		content_type[32];	
}Route;

typedef char* (*handlerfp)(char*);

#endif

