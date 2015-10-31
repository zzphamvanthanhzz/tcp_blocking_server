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

#define MAXHOSTNAME 256
using namespace std;

main() {
	struct sockaddr_in socketInfo;
	char sysHost[MAXHOSTNAME + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = 8081;

	bzero(&socketInfo, sizeof (sockaddr_in)); // Clear structure memory

	// Get system information

	gethostname(sysHost, MAXHOSTNAME); // Get the name of this computer we are running on
	if ((hPtr = gethostbyname(sysHost)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		exit(EXIT_FAILURE);
	}

	// create socket

	if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		close(socketHandle);
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
	socketInfo.sin_port = htons(portNumber); // Set port number

	// Bind the socket to a local socket address

	if (bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof (socketInfo)) < 0) {
		close(socketHandle);
		perror("bind");
		exit(EXIT_FAILURE);
	}

	listen(socketHandle, 5);

	int socketConnection;
	int i = 1;
	do {
		if ((socketConnection = accept(socketHandle, NULL, NULL)) < 0) {
			perror("Error accept");
			exit(EXIT_FAILURE);
		}
		printf("Receive Request %d\n", i);
		int rc = 0; // Actual number of bytes read
		char buf[512];

		// rc is the number of characters returned.
		// Note this is not typical. Typically one would only specify the number 
		// of bytes to read a fixed header which would include the number of bytes
		// to read. See "Tips and Best Practices" below.

		rc = recv(socketConnection, buf, 512, 0);
		buf[rc] = (char) NULL; // Null terminate string
		//Sleep
		sleep(5);
		cout << "Number of bytes read: " << rc << endl;
		cout << "Received: " << buf << endl;
		i++;

		send(socketConnection, buf, strlen(buf) + 1, 0);
	} while (1);
	return 0;
}


