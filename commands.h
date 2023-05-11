#pragma once

#include "command_parsing.h"

bool register_user(auth_info_t info, int sockfd);
bool login_user(auth_info_t info, int sockfd);
bool enter_library(int sockfd);
bool get_books(int sockfd);
bool get_book(int id, int sockfd);
bool add_book(book_t book, int sockfd);
bool delete_book(int id, int sockfd);
bool logout(int sockfd);
bool exit_client(int sockfd);
