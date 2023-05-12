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

	std::cout  << get_json_body(response) << "\n";
}

void get_book(int id, int sockfd) {
	std::string string_id = std::to_string(id);
	std::string url(BOOKS_ROOT);
	url.append(string_id);

	char *message = compute_get_request(HOST, url.c_str(), user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

	std::cout  << get_json_body(response) << "\n";
}

void add_book(book::info_t book, int sockfd) {
	json body = book;

    char *message = compute_post_request(HOST, ALL_BOOKS, JSON_BODY, body.dump().c_str(),
										user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

    std::cout << "Successfully added book \"" << book.title << "\".\n";
}

void delete_book(int id, int sockfd) {
	std::string string_id = std::to_string(id);
	std::string url(BOOKS_ROOT);
	url.append(string_id);

	char *message = compute_delete_request(HOST, url.c_str(), user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

    std::cout << "Successfully deleted book.\n";
}

void logout(int sockfd) {
	char *message = compute_get_request(HOST, LOGOUT, user.cookies, user.auth_token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

	/* Handle errors, if they occur */
	std::optional<std::string> error = get_error(response);
	if (error.has_value()) {
		std::cout << error.value() << "\n";
		return;
	}

	user.cookies.clear();
	user.auth_token.clear();

	std::cout  << "Successfully logged out.\n";
}

void invalid_command() {
	std::cout << "Invalid command" << "\n";
}

void exit_client(int sockfd) {
	close(sockfd);
	exit(0);
}