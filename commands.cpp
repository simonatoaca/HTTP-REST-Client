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

bool register_user(auth_info_t info, int sockfd) {
	std::cout << info.username << " " << info.password << " " << sockfd << std::endl;
    return true;
}

bool login_user(auth_info_t info, int sockfd) {
	std::cout << info.username << " " << info.password << " " << sockfd << std::endl;
    return true;
}

bool enter_library(int sockfd) {
    std::cout << "Trying to enter the library" << std::endl;
    return true;
}

bool get_books(int sockfd) {
    return true;
}

bool get_book(int id, int sockfd) {
    return true;
}

bool add_book(book_t book, int sockfd) {
    std::cout << "Adding book " <<  book.title << " published by " << book.author << std::endl;

    return true;
}

bool delete_book(int id, int sockfd) {
    return true;
}

bool logout(int sockfd) {
    std::cout << "Logging out" << std::endl;
    return true;
}

bool exit_client(int sockfd) {
	close(sockfd);
    return false;
}