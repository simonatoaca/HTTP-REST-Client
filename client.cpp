#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

#include "command_parsing.h"

using json = nlohmann::json;

namespace ns {
	// a simple struct to model a person
	struct person {
		std::string name;
		std::string address;
		int age;
	};

	void to_json(json& j, const person& p) {
		j = json{ {"name", p.name}, {"address", p.address}, {"age", p.age} };
	}

	void from_json(const json& j, person& p) {
		j.at("name").get_to(p.name);
		j.at("address").get_to(p.address);
		j.at("age").get_to(p.age);
	}
}

int main() {
	// ns::person p = {"Ned Flanders", "744 Evergreen Terrace", 60};

	// // conversion: person -> json
	// json j = p;

	// std::cout << j << std::endl;

	std::string command;
	while (1) {
		std::getline(std::cin, command);

		if (parse_command(command)) {
			break;
		}
	}
}