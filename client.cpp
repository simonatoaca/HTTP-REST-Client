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
#include <sys/epoll.h>

#include "parsers/command_parsing.h"
#include "utils/requests.h"
#include "client.h"

#define MAX_CONNECTIONS 2
#define EPOLL_TIMEOUT 1000

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

int add_to_epoll(int epollfd, int fd, int flag) {
	struct epoll_event ev;

	ev.events = flag;
	ev.data.fd = fd;

	return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

int remove_from_epoll(int epollfd, int fd, int flag) {
	struct epoll_event ev;

	ev.events = flag;
	ev.data.fd = fd;

	return epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

int main() {
	int rc;

	int epollfd = epoll_create(MAX_CONNECTIONS);

	/* Add descriptor to epoll */
	add_to_epoll(epollfd, STDIN_FILENO, EPOLLIN);

	std::string command;
	bool can_send = false;
	std::optional<std::function<void (int)>> cmd = {};

	while (1) {
		struct epoll_event rev;

		rc = epoll_wait(epollfd, &rev, 1, EPOLL_TIMEOUT);
		DIE(rc < 0, "epoll_wait");

		/* Connection timed out / was closed */
		if (rev.data.fd == sockfd && (rev.events & EPOLLRDHUP)) {
			/* If there was a command waiting, reopen the connection */
			if (cmd.has_value()) {
				close(sockfd);
				sockfd = init_connection();

				cmd.value()(sockfd);
				cmd = {};
				continue;
			}

			/* Close connection and set flag */
			close(sockfd);
			remove_from_epoll(epollfd, sockfd, EPOLLRDHUP | EPOLLOUT);
			can_send = false;

			continue;
		}

		/* Received input from the user */
		if (rev.data.fd == STDIN_FILENO && (rev.events & EPOLLIN)) {
			std::getline(std::cin, command);
			cmd = parse_command(command);

			/* Reopen connection if needed */
			if (!can_send) {
				sockfd = init_connection();
				add_to_epoll(epollfd, sockfd, EPOLLRDHUP | EPOLLOUT);
				can_send = true;
			}

			/* Wait for epollout / epollrdhup from the server */
			rc = epoll_wait(epollfd, &rev, 1, EPOLL_TIMEOUT);
			DIE(rc < 0, "epoll_wait");

			if (rev.data.fd == sockfd && (rev.events & EPOLLRDHUP)) {
				close(sockfd);
				sockfd = init_connection();
			}

			cmd.value()(sockfd);
			cmd = {};
			continue;
		}
	}
}