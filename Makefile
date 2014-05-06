labyrinth: main.cpp header.cpp init.cpp gen.cpp game.cpp turn.cpp debug.cpp save.cpp server.cpp
	g++ -std=c++0x -O2 -Wall -Wextra -Wno-unused-result -o labyrinth -l pthread main.cpp
