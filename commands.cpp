#include <iostream>
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <optional>

#include "commands.h"
#include "requests.h"
#include "http_parser.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static user_t user;

void register_user(auth::info_t info, int sockfd) {
	json body = info;
    char *message = compute_post_request(HOST, REGISTER, JSON_BODY, body.dump().c_str(),
										user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

    std::cout << "Successfully registered username " << info.username << "!\n";
}

void login_user(auth::info_t info, int sockfd) {
	json body = info;

    char *message = compute_post_request(HOST, LOGIN, JSON_BODY, body.dump().c_str(),
										user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

	/* Get cookie */
	user.cookies.push_back(get_cookie(response));

    std::cout << "Successfully logged in!\n";
}

void enter_library(int sockfd) {
	char *message = compute_get_request(HOST, LIBRARY, user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

	/* Get token */
	user.auth_token = get_token(response);

	std::cout << "Access granted into the library.\n";
}

void get_books(int sockfd) {
	char *message = compute_get_request(HOST, ALL_BOOKS, user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

	puts(response);
	std::cout << "\n";
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