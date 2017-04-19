#include <stdlib.h>          // for atoi
#include <unistd.h>          // for write

#include <bcc/bpf_common.h>  // for bpf_function_start, bpf_module_create_c
#include <bcc/libbpf.h>      // for bpf_attach_socket

#include "common.h"          // for die, bind_socket, get_socket, udp_lookup

int main (int argc, char** argv) {
	char buffer[4096];

	struct addrinfo* addr = udp_lookup ("::1", "4242");

	int sockfd = get_socket(addr);

	bind_socket (sockfd, addr);

	if (atoi(argv[1])) {
		void * program = bpf_module_create_c("filter.c", 0, NULL, 0);
		void * start = bpf_function_start (program, "filter");
		size_t size = bpf_function_size (program, "filter");
		int progfd = bpf_prog_load(BPF_PROG_TYPE_SOCKET_FILTER, start, size, "GPL", 0, NULL, 0);

		int r = bpf_attach_socket(sockfd, progfd);

		if (r != 0) {
			die ("bpf_attach_socket() failed");
		}
	}

	for (;;) {
		size_t bytes = recv (sockfd, buffer, sizeof(buffer), 0);
		write (1, buffer, bytes);
	}
}
