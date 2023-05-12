#include "http_parser.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <optional>
#include <regex>
#include <nlohmann/json.hpp>

static std::regex cookie_regex("(Set-Cookie: ).*");
static std::regex cookie_replace("(Set-Cookie: )");
static std::regex error_regex("\"(error)\":\"((\\\"|[^\"])*)\"");
static std::regex token_regex("\"(token)\":\"((\\\"|[^\"])*)\"");
static std::regex parse_json_field("\"[^\"]*\"");
static std::regex json_body("(\\[|\\{)(\\{|\").*");


std::string get_cookie(std::string response) {
	/* Match set-cookie line in response */
	std::smatch match;
	std::regex_search(response, match, cookie_regex);

	/* Return what is after : in set-cookie */
	std::string cookie = std::regex_replace(match[0].str(), cookie_replace, "$2");
	return cookie;
}

std::optional<std::string> get_error(std::string response) {
	std::smatch match;
	if (std::regex_search(response, match, error_regex)) {
		std::string error = std::regex_replace(match[2].str(), parse_json_field, "$2");
		return error;
	}

	return {};
}

std::string get_token(std::string response) {
	std::smatch match;
	std::regex_search(response, match, token_regex);
	std::string token = std::regex_replace(match[2].str(), parse_json_field, "$2");
	return token;
}

std::string get_json_body(std::string response) {
	std::smatch match;
	std::regex_search(response, match, json_body);
	return match[0].str();
}

