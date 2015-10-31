/* 
 * File:   tcp_nonblocking_server.cpp
 * Author: thanhpv
 * http://www.cs.tau.ac.il/~eddiea/samples/Non-Blocking/tcp-nonblocking-server.c.html
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 * Error temporary source: http://stackoverflow.com/questions/14370489/what-can-cause-a-resource-temporarily-unavailable-on-sock-send-command#comment19984577_14370489
 * Created on October 27, 2015, 11:35 PM
 */
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXHOSTNAME 256
#define PORT 9090
#define BUFFSIZE 512
using namespace std;

main() {
	int sock_listen;
	struct sockaddr_in sockInfo;
	struct sockaddr_in clientSockInfo;
	socklen_t client_size;
	//Create socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error create socket");
		exit(-1);
	}
	//Bind
	bzero(&sockInfo, sizeof (sockaddr_in)); // Clear structure memory
	sockInfo.sin_family = AF_INET;
	sockInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	sockInfo.sin_port = htons(PORT);

	if (bind(sock_listen, (struct sockaddr*) &sockInfo, sizeof (sockInfo)) == -1) {
		perror("Error binding");
		exit(-1);
	}
	//Listen
	if (listen(sock_listen, 1) == -1) {
		perror("Error listen");
		exit(-1);
	}
	//Connect
	int sock_connect; //accept, receive and send via this socket fd
	int rc, sd, size_recv; //number bytes received, send
	char buff_recv[BUFFSIZE];
	char buff_send[BUFFSIZE];
	sockaddr_in sockRecvInfo;
	while (1) {
		//		The  accept()  system call is used with connection-based
		//       socket types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts
		//       the  first  connection  request  on the queue of pending
		//       connections for the listening socket, sockfd, creates  a
		//       new  connected socket, and returns a new file descriptor
		//       referring to that socket.  The newly created  socket  is
		//       not  in the listening state.  The original socket sockfd
		//       is unaffected by this call.
		//
		//       The argument sockfd is a socket that  has  been  created
		//       with  socket(2),  bound to a local address with bind(2),
		//       and is listening for connections after a listen(2).
		//
		//       The argument addr is a pointer to a sockaddr  structure.
		//       This structure is filled in with the address of the peer
		//       socket, as known to the communications layer.  The exact
		//       format of the address returned addr is determined by the
		//       socket’s address family (see socket(2) and  the  respec‐
		//       tive protocol man pages).  When addr is NULL, nothing is
		//       filled in; in this case, addrlen is not used, and should
		//       also be NULL.
		//
		//       The  addrlen  argument  is  a value-result argument: the
		//       caller must initialize it to contain the size (in bytes)
		//       of  the  structure pointed to by addr; on return it will
		//       contain the actual size of the peer address.
		//
		//       The returned address is truncated if the buffer provided
		//       is  too small; in this case, addrlen will return a value
		//       greater than was supplied to the call.
		//
		//       If no pending connections are present on the queue,  and
		//       the socket is not marked as nonblocking, accept() blocks
		//       the caller until a connection is present.  If the socket
		//       is  marked  nonblocking  and  no pending connections are
		//       present on the queue,  accept()  fails  with  the  error
		//       EAGAIN or EWOULDBLOCK.
		if ((sock_connect = accept(sock_listen, (struct sockaddr*) &clientSockInfo, &client_size)) == -1) {
			perror("Error accept");
			continue;
		}
		//Process data
		rc = recv(sock_connect, &buff_recv, BUFFSIZE, 0);
		if (rc < 0) {
			perror("Error receive");
			exit(-1);
		}
		printf("DATA RECEIVE IS: %s\tSIZE: %ld\n", buff_recv, strlen(buff_recv));
		//inet_ntoa to convert ip
		//ntohs to convert port
		printf("DATA RECEIVE FROM HOST: %s\t PORT: %d\n", inet_ntoa(sockRecvInfo.sin_addr), ntohs(sockRecvInfo.sin_port));
		strcpy(buff_send, "Welcome");

		sd = send(sock_connect, &buff_send, strlen(buff_send) + 1, 0);
		if (send < 0) {
			perror("Error send to client");
			exit(-1);
		}
	}


	return 0;
}


