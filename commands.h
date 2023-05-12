#pragma once

#include "command_parsing.h"

#define JSON_BODY "application/json"
#define HOST "34.254.242.81:8080"
#define LOGIN "/api/v1/tema/auth/login"
#define REGISTER "/api/v1/tema/auth/register"
#define LIBRARY "/api/v1/tema/library/access"
#define ALL_BOOKS "/api/v1/tema/library/books"

typedef struct {
    std::vector<std::string> cookies;
    std::string auth_token;
} user_t;



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
