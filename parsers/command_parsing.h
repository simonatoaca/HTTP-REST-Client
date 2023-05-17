#pragma once

#include <string>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace auth {
    typedef struct {
        std::string username;
        std::string password;
    } info_t;

    void to_json(json& j, const info_t& info);
	void from_json(const json& j, info_t& info);
}

namespace book {
    typedef struct {
        int id;
        std::string title;
        std::string author;
        std::string genre;
        std::string publisher;
        int page_count;
    } info_t;

    void to_json(json& j, const info_t& info);
	void from_json(const json& j, info_t& info);
}


/* Returns the function called by command */
std::function<void(int)> parse_command(std::string& command);