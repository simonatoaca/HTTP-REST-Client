#include <iostream>
#include <regex>
#include <functional>

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#include "command_parsing.h"
#include "client.h"
#include "commands.h"

#define CMD_RANGE "^(register|login|(add|delete)?_book|get_book[s]{0,1}|enter_library|logout|exit)$"

static std::regex cmd_regex(CMD_RANGE);

auth_info_t get_auth_info() {
	std::string username, password;

	std::cout << "username: ";
	std::getline(std::cin, username);

	std::cout << "password: ";
	std::getline(std::cin, password);

	return {username, password};
}

int get_book_id() {
	std::string id;
	std::cout << "id: ";
	std::getline(std::cin, id);

	std::cout << "id is " << id << std::endl;

	return stoi(id);
}

book_t get_book_info() {
	std::string title, author, genre, publisher, page_count;
	std::cout << "title: ";
	std::getline(std::cin, title);

	std::cout << "author: ";
	std::getline(std::cin, author);

	std::cout << "genre: ";
	std::getline(std::cin, genre);

	std::cout << "publisher: ";
	std::getline(std::cin, publisher);

	std::cout << "page_count: ";
	std::getline(std::cin, page_count);

	return {title, author, genre, publisher, stoi(page_count)};
}

std::function<bool(int)> parse_command(std::string& command) {
	if (!std::regex_search(command, cmd_regex)) {
		std::cout << "Invalid command" << std::endl;
	}

	if (command == "register") {
		auth_info_t auth_info = get_auth_info();
		return [auth_info](int sockfd) {return register_user(auth_info, sockfd);};
	}

	if (command == "login") {
		auth_info_t auth_info = get_auth_info();
		return [auth_info](int sockfd) {return login_user(auth_info, sockfd);};
		//return std::bind(login_user, auth_info, std::placeholders::_1);
	}

	if (command == "enter_library") {
		return [](int sockfd) {return enter_library(sockfd);};
	}

	if (command == "get_books") {
		return [](int sockfd) {return get_books(sockfd);};
	}

	if (command == "get_book") {
		int id = get_book_id();
		return [id](int sockfd) {return get_book(id, sockfd);};
	}

	if (command == "delete_book") {
		int id = get_book_id();
		return [id](int sockfd) {return delete_book(id, sockfd);};
	}

	if (command == "add_book") {
		book_t book_info = get_book_info();
		return [book_info](int sockfd) {return add_book(book_info, sockfd);};
	}

	if (command == "logout") {
		return [](int sockfd) {return logout(sockfd); };
	}

	return [](int sockfd) {return exit_client(sockfd);};
}