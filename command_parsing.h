#pragma once

#include <string>

typedef struct {
	std::string username;
	std::string password;
} auth_info_t;

auth_info_t get_auth_info();

/* Returns true if exit was encountered */
bool parse_command(std::string command);