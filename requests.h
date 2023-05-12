#pragma once
#include <iostream>
#include <regex>
#include <string>
#include <functional>

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <vector>


#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

#define BUFLEN 4096
#define LINELEN 1000

typedef struct {
    char *data;
    size_t size;
} buffer;

void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
char *compute_get_request(const char *host, const char *url, std::vector<std::string> &cookies,
						 std::string authorization_token);
char *compute_post_request(const char *host, const char *url, const char* content_type, const char *body_data,
							std::vector<std::string> &cookies, std::string authorization_token);