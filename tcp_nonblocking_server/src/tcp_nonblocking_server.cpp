/* 
 * File:   tcp_nonblocking_server.cpp
 * Author: thanhpv
 * http://www.cs.tau.ac.il/~eddiea/samples/Non-Blocking/tcp-nonblocking-server.c.html
 * Created on October 27, 2015, 11:35 PM
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <strings.h>
#include <list> /* Added for the nonblocking socket */

#define PORT 6060
#define BACKLOG 10

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr; /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	int sin_size;
	char string_read[255];
	int n, i;
	int last_fd; /* Thelast sockfd that is connected	*/

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Error create socket %s\n", strerror(errno));
		return -1;
	}
	last_fd = sockfd;

	//
	my_addr.sin_family = AF_INET; /* host byte order */
	my_addr.sin_port = htons(PORT); /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* Address to accept any incoming messages.  */
	bzero(&(my_addr.sin_zero), 8); /* zero the rest of the struct */

	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//Assigning a name to a socket
	//when a socket is created with socket()
	//it exists in a namespace (address family) but has no address assigned to it.
	//bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd.
	//addrlen specifies the size, in bytes, of the address structure pointed to by addr
	if (bind(sockfd, (struct sockaddr*) &my_addr, sizeof (struct sockaddr)) == -1) {
		printf("Error bind %s\n", strerror(errno));
		return -1;
	}

	//int listen(int sockfd, int backlog);
	//marks the socket referred to by sockfd as a passive socket
	//as a socket that will be used to accept incoming connection requests using accept()
	//The sockfd argument is a file descriptor that refers to a socket of type SOCK_STREAM or SOCK_SEQPACKET.
	//The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
	//If a connection request arrives when  the queue is full, the client may receive an error 
	//with an indication of ECONNREFUSED  if the underlying protocol supports retransmission, the request may
	//be ignored so that a later reattempt at connection succeeds.
	if (listen(sockfd, BACKLOG) == -1) {
		printf("Error listen %s\n", strerror(errno));
		return -1;
	}

	//sockfd: Specifies a socket that was created with socket(), has been bound
	//to an address with bind(), and has issued a 
	//address: Either a null pointer, or a pointer to a sockaddr structure 
	//where the address of the connecting socket shall be returned
	//address_len: Points to a socklen_t structure which on input specifies the 
	//length of the supplied sockaddr structure, and on output 
	//specifies the length of the stored address.
	if ((new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size)) == -1) {
		printf("Error accept %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

