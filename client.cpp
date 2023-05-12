#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <time.h>

#include "command_parsing.h"
#include "requests.h"

static int sockfd;

int init_connection() {
	struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
		perror("socket init failed");
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_aton("34.254.242.81", &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
	}
	
	return sockfd;
}

int main() {
	std::string command;
	while (1) {
		std::getline(std::cin, command);

		sockfd = init_connection();
		parse_command(command)(sockfd);
		close(sockfd);
	}
}