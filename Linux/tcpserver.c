#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

void *connection_handler(void *);




int main() {


// create the server socket
int server_socket,*new_socket;
server_socket = socket (AF_INET, SOCK_STREAM, 0 );

// define the server address
struct sockaddr_in server_address;
server_address.sin_family = AF_INET;
server_address.sin_port = htons (9002);
server_address.sin_addr.s_addr = INADDR_ANY;

// bind the socket to our specified IP and port
bind( server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
listen(server_socket, 5);
int client_socket;
while((client_socket = accept (server_socket, NULL, NULL))){
	
	pthread_t thread;
		new_socket = malloc(1);
		*new_socket = client_socket;
		
		if( pthread_create( &thread , NULL ,  connection_handler , (void*) new_socket) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		pthread_join(thread , NULL);
	
	}


return 0;
}



void *connection_handler(void *client_socket)
{
	char server_message[256] = "You have reached the server!";
	int sock = *(int*)client_socket;
// send the message
send(sock, server_message, sizeof(server_message), 0);

//receive the message
char client_response[256];
recv(sock, &client_response, sizeof(client_response), 0);
printf("The client sent the data: %s\n", client_response);

// close the socket
//close(server_socket);
	
return 0;
}
