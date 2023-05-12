FILES=client.cpp parsers/*.cpp utils/*.cpp
FLAGS=-std=c++17 -Wall -Werror

build: client

client: $(FILES)
	g++ $(FLAGS) $^ -o $@

clean:
	rm client

zip:
	zip Toaca_Alexandra_322CA_Tema3PC.zip *.cpp parsers/* utils/* *.h Makefile README.md