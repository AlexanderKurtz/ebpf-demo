#include <netdb.h>   // for freeaddrinfo
#include <stdlib.h>  // for atoi

#include "common.h"  // for get_socket, udp_lookup

int main (int argc, char** argv) {
	size_t bytes = atoi (argv[1]);
	char* buffer = malloc (bytes);

	struct addrinfo* server = udp_lookup ("::1", "4242");
	int sockfd = get_socket(server);

	for (;;) {
		sendto (sockfd, buffer, bytes, 0, server->ai_addr, server->ai_addrlen);
	}

	freeaddrinfo (server);
}
