#include <sys/socket.h>
#include "constants.h"

int	CreateSocket	(void);
void	BindSocket	(int socket_id, struct sockaddr_in* address, int addr_size);
void	ListenToSocket	(int socket_id, int backlog_size);
int	AcceptSocket	(int socket_id, struct sockaddr* address, socklen_t* addr_size);
void	CloseSocket	(int socket_id);
