#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <time.h>

#include "command_parsing.h"

using json = nlohmann::json;

static int sockfd;

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

int init_connection() {
	struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
		perror("socket init failed");
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_aton("34.254.242.81", &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
	}
	
	return sockfd;
}

int main() {
	// ns::person p = {"Ned Flanders", "744 Evergreen Terrace", 60};

	// // conversion: person -> json
	// json j = p;

	// std::cout << j << std::endl;
	sockfd = init_connection();

	std::string command;
	while (1) {
		std::getline(std::cin, command);

		clock_t clk = clock();
		parse_command(command)(sockfd);
		std::cout << clock() - clk << std::endl;
	}
}