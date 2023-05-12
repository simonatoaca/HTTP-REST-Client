#include <iostream>
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#include "commands.h"
#include "requests.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

void register_user(auth::info_t info, int sockfd) {
	json body = info;

	std::cout << "\n";
    char *message = compute_post_request(HOST, REGISTER, JSON_BODY, body.dump().c_str(), NULL, 0);
    puts(message);
	std::cout << "\n";

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */

    puts(response);
	std::cout << "\n";
}

void login_user(auth::info_t info, int sockfd) {
	json body = info;

	std::cout << "\n";
    char *message = compute_post_request(HOST, LOGIN, JSON_BODY, body.dump().c_str(), NULL, 0);
    puts(message);
	std::cout << "\n";

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */

	/* Get cookie */

    puts(response);
	std::cout << "\n";
}

void enter_library(int sockfd) {
	std::cout << "Trying to enter the library" << "\n";
}

void get_books(int sockfd) {
}

void get_book(int id, int sockfd) {
	std::cout << "Getting book " << id << "\n";
}

void add_book(book::info_t book, int sockfd) {
	json j = book;
	std::cout << j << std::endl;
}

void delete_book(int id, int sockfd) {
}

void logout(int sockfd) {
	std::cout << "Logging out" << "\n";
}

void invalid_command() {
	std::cout << "Invalid command" << "\n";
}

void exit_client(int sockfd) {
	close(sockfd);
	exit(0);
}