#pragma once

#include "command_parsing.h"

void register_user(auth::info_t info, int sockfd);
void login_user(auth::info_t info, int sockfd);
void enter_library(int sockfd);
void get_books(int sockfd);
void get_book(int id, int sockfd);
void add_book(book::info_t book, int sockfd);
void delete_book(int id, int sockfd);
void logout(int sockfd);
void invalid_command();
void exit_client(int sockfd);
