#pragma once

#include <string>
#include <optional>

std::string get_cookie(std::string response);
std::optional<std::string> get_error(std::string response);
std::string get_token(std::string response);
std::string get_json_body(std::string response);