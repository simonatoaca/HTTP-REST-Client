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

#include "utils/connection_utils.h"
#include "parsers/command_parser.h"
#include "utils/requests.h"
#include "client.h"

static int sockfd;

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