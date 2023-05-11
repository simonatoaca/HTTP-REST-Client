#include <iostream>
#include <regex>
#include <functional>
#include <optional>

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
static std::regex auth_regex("^[^ ]*$");
static std::regex id_regex("^[0-9]*$");

void auth::to_json(json& j, const info_t& info) {
	j = json{ {"username", info.username}, {"password", info.password}};
}

void auth::from_json(const json& j, info_t& info) {
	j.at("username").get_to(info.username);
	j.at("password").get_to(info.password);
}

void book::to_json(json& j, const info_t& info) {
	j = json{{"title", info.title}, {"author", info.author}, {"genre", info.genre},
				{"publisher", info.publisher}, {"page_count", info.page_count}};
}

void book::from_json(const json& j, info_t& info) {
	j.at("title").get_to(info.title);
	j.at("author").get_to(info.author);
	j.at("genre").get_to(info.genre);
	j.at("publisher").get_to(info.publisher);
	j.at("page_count").get_to(info.page_count);
}

std::optional<auth::info_t> get_auth_info() {
	std::string username, password;

	std::cout << "username: ";
	std::getline(std::cin, username);

	if (!std::regex_search(username, auth_regex)) {
		std::cout << "Invalid username!\n";
		return {};
	}

	std::cout << "password: ";
	std::getline(std::cin, password);

	if (!std::regex_search(password, auth_regex)) {
		std::cout << "Invalid password!\n";
		return {};
	}

	return (auth::info_t){username, password};
}

std::optional<int> get_book_id() {
	std::string id;
	std::cout << "id: ";
	std::getline(std::cin, id);

	if (!std::regex_search(id, id_regex)) {
		std::cout << "Invalid id! Please enter a number.\n";
		return {};
	}

	return stoi(id);
}

std::optional<book::info_t> get_book_info() {
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

	if (!std::regex_search(page_count, id_regex)) {
		std::cout << "Invalid page count! Please enter a number.\n";
		return {};
	}

	return (book::info_t){title, author, genre, publisher, stoi(page_count)};
}

std::function<void(int)> parse_command(std::string& command) {
	if (!std::regex_search(command, cmd_regex)) {
		return [](int sockfd) {return invalid_command(); };
	}

	if (command == "register") {
		std::optional<auth::info_t> auth_info = get_auth_info();
		if (!auth_info.has_value()) {
			return [](int sockfd) {};
		}

		return [auth_info](int sockfd) {return register_user(auth_info.value(), sockfd);};
	}

	if (command == "login") {
		std::optional<auth::info_t> auth_info = get_auth_info();

		if (!auth_info.has_value()) {
			return [](int sockfd) {};
		}

		return [auth_info](int sockfd) {return login_user(auth_info.value(), sockfd);};
	}

	if (command == "enter_library") {
		return [](int sockfd) {return enter_library(sockfd);};
	}

	if (command == "get_books") {
		return [](int sockfd) {return get_books(sockfd);};
	}

	if (command == "get_book") {
		std::optional<int> id = get_book_id();

		if (!id.has_value()) {
			return [](int sockfd) {};
		}
		return [id](int sockfd) {return get_book(id.value(), sockfd);};
	}

	if (command == "delete_book") {
		std::optional<int> id = get_book_id();
		if (!id.has_value()) {
			return [](int sockfd) {};
		}

		return [id](int sockfd) {return delete_book(id.value(), sockfd);};
	}

	if (command == "add_book") {
		std::optional<book::info_t> book_info = get_book_info();

		if (!book_info.has_value()) {
			return [](int sockfd) {};
		}

		return [book_info](int sockfd) {return add_book(book_info.value(), sockfd);};
	}

	if (command == "logout") {
		return [](int sockfd) {return logout(sockfd); };
	}

	return [](int sockfd) {return exit_client(sockfd);};
}