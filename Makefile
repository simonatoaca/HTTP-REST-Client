FILES=client.cpp command_parsing.cpp commands.cpp requests.cpp http_parser.cpp
FLAGS=-std=c++17 -Wall -Werror

build: client

client: $(FILES)
	g++ $(FLAGS) $^ -o $@

clean:
	rm client

zip:
	zip Toaca_Alexandra_322CA_Tema3PC.zip *.cpp *.h Makefile README.md