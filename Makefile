FILES=client.cpp command_parsing.cpp commands.cpp
FLAGS=-std=c++17 -Wall -Werror

build: client

client: $(FILES)
	g++ $(FLAGS) $^ -o $@

clean:
	rm client