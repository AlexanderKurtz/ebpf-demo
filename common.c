#include <netdb.h>       // for addrinfo, getaddrinfo
#include <stdio.h>       // for NULL, perror
#include <stdlib.h>      // for exit
#include <sys/socket.h>  // for bind, socket, SOCK_DGRAM

#include "common.h"      // for die, udp_lookup, get_socket, bind_socket

/* Print an error message and terminate the program */
void die (const char* message) {
	perror (message);
	exit (1);
}

/* UDP-only getaddrinfo() with error checking. */
struct addrinfo* udp_lookup (const char* addr, const char* port){
	struct addrinfo* result = NULL;

	struct addrinfo hints = {
		.ai_flags     = 0,
		.ai_family    = 0,
		.ai_socktype  = SOCK_DGRAM,
		.ai_protocol  = 0,
		.ai_addrlen   = 0,
		.ai_addr      = NULL,
		.ai_canonname = NULL,
		.ai_next      = NULL,
	};

	int r = getaddrinfo (addr, port, &hints, &result);

	if (r != 0) {
		die ("getaddrinfo() failed");
	}

	if (result == NULL) {
		die ("getaddrinfo() returned no addresses");
	}

	return result;
}

/* socket() with error checking. */
int get_socket (struct addrinfo* addr) {
	int result = socket (addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if (result < 0) {
		die ("socket() failed");
	}

	return result;
}

/* bind() with error checking. */
void bind_socket (int sockfd, struct addrinfo* addr) {
	int r = bind (sockfd, addr->ai_addr, addr->ai_addrlen);

	if (r != 0) {
		die ("bind() failed");
	}
}
