#include "http_parser.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <nlohmann/json.hpp>

// static std::regex cookie_regex("^.*(Set-Cookie: ).*$");
static std::regex cookie_regex("(Set-Cookie: ).*");
static std::regex cookie_replace("(Set-Cookie: )");


std::string get_cookie(std::string response) {
	/* Match set-cookie line in response */
	std::smatch match;
	std::regex_search(response, match, cookie_regex);

	/* Return what is after : in set-cookie */
	std::string cookie = std::regex_replace(match[0].str(), cookie_replace, "$2");
	return cookie;
}

