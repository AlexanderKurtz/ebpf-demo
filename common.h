#pragma once

#include <netdb.h> // for struct addrinfo

void die (const char* message);
struct addrinfo* udp_lookup (const char* addr, const char* port);
int get_socket (struct addrinfo* addr);
void bind_socket (int sockfd, struct addrinfo* addr);
