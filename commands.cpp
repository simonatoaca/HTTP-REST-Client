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

void register_user(auth::info_t info, int sockfd) {
	std::cout << info.username << " " << info.password << " " << sockfd << "\n";

}

void login_user(auth::info_t info, int sockfd) {
	std::cout << info.username << " " << info.password << " " << sockfd << "\n";
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
	std::cout << "Adding book " <<  book.title << " published by " << book.author << "\n";
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