#include <iostream>
#include <regex>

#include "command_parsing.h"
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

bool parse_command(std::string command) {
    if (!std::regex_search(command, cmd_regex)) {
        std::cout << "Invalid command" << std::endl;
        return false;
    }

    if (command == "exit") {
        return true;
    }

    if (command == "register") {
        const auto& [username, password] = get_auth_info();
	    std::cout << username << " was added with password " << password << std::endl;
        return false;
    }

    if (command == "login") {
        const auto& [username, password] = get_auth_info();
	    std::cout << username << " was added with password " << password << std::endl;
        return false;
    }

    std::cout << "Command is " << command << std::endl;
    

    return false;
}