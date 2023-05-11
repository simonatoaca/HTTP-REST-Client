#pragma once

#include <string>
#include <functional>
#include <variant>

typedef struct {
	std::string username;
	std::string password;
} auth_info_t;

typedef struct {
    std::string title;
    std::string author;
    std::string genre;
    std::string publisher;
    int page_count;
} book_t;

auth_info_t get_auth_info();

/* Returns true if exit was encountered */
std::function<bool(int)> parse_command(std::string& command);