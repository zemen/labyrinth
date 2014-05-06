#ifndef INIT_INCLUDED

#define INIT_INCLUDED

#include "header.cpp"

void incorrect_information() {
	cout << "It's incorrect information" << endl;
	cout << "Bye, loser" << endl;
	exit(0);
}

void set_standard_labyrinth(Parameters &params) {
	params.size = 5;
	params.minotaurs = 3;
	params.portals = 3;
	params.exits = 2;
	params.bullets = 2;
	params.bombs = 2;
}

void set_labyrinth_for_server(Parameters &params) {
	set_standard_labyrinth(params);
	params.player_names.clear();
	params.players = 0;
}

void get_players(Parameters &params, int max_number) {
	cout << "Enter number of players (from 1 to " << max_number << ")" << endl;
	cin >> params.players;
	if (params.players <= 0 || params.players > max_number)
		incorrect_information();
	
	cout << "Enter names of players(separated with spaces or new lines)" << endl;
	cout << "All names should be unique and not longer than 100 characters" << endl;
	
	set <string> names;
	names.clear();
	params.player_names.resize(params.players);
	for (int i = 0; i < params.players; i++) {
		cin >> params.player_names[i];
		if (params.player_names[i].size() > 100)
			incorrect_information();
		names.insert(params.player_names[i]);
	}
	if ((int) names.size() != params.players)
		incorrect_information();
}

Parameters get_information() {
	string trash;
	Parameters params;
	cout << "Would you like to play standard game or custom? (s - standard, c - custom)" << endl;
	string mode;
	cin >> mode;
	if (mode != "s" && mode != "c")
		incorrect_information();
	if (mode == "s") {
		set_standard_labyrinth(params);
		get_players(params, params.size * params.size - params.portals - params.minotaurs - 1);
		return params;
	}
	
	cout << "Enter custom labyrinth parameters" << endl;
	cout << "Enter the size of the labyrinth (from 2 to " << MAX_SIZE << ")" << endl;
	cin >> params.size;
	if (params.size < 2 || params.size > MAX_SIZE)
		incorrect_information();
	
	get_players(params, params.size * params.size - 2);
	int empty_cells = params.size * params.size - params.players;
	
	cout << "Enter number of Minotaurs (from 1 to " << empty_cells - 1 << ")" << endl;
	cin >> params.minotaurs;
	if (params.minotaurs < 1 || params.minotaurs > empty_cells)
		incorrect_information();
	empty_cells -= params.minotaurs;
	
	cout << "Enter number of exits (from 1 to " << params.size * 4 << ")" << endl;
	cin >> params.exits;
	if (params.exits < 1 || params.exits > params.size * 4)
		incorrect_information();
	
	cout << "Enter number of portals (from 0 to " << empty_cells - 1 << ", but not 1)" << endl;
	cin >> params.portals;
	if (params.portals < 0 || params.portals == 1 || params.portals > empty_cells - 1)
		incorrect_information();
	
	cout << "Enter initial number of bullets for every player" << endl;
	cin >> params.bullets;
	
	cout << "Enter initial number of bombs for every player" << endl;
	cin >> params.bombs;
	
	getline(cin, trash);
	params.check();
	return params;
}

void init() {
	srand(time(0));
	
	idiot_string.push_back("you're an idiot!");
	idiot_string.push_back("are you an idiot?");
	idiot_string.push_back("have you got a brain?");
	idiot_string.push_back("are you crazy?");
	idiot_string.push_back("only the wall can help you.");
	idiot_string.push_back("you should drink a poison.");
	idiot_string.push_back("you're a bit stupid.");
	idiot_string.push_back("do you speak English?");
	idiot_string.push_back("WTF?");
	idiot_string.push_back("it's better to kill yourself, idiot!");
	idiot_string.push_back("this game is too hard for you.");
	
	if (DEBUG)
		cout << "Debug mod on" << endl;
	cout << "Hello, world!" << endl;
	cout << "This program is made by pershik && matushkin && zemen" << endl
			<< "Please, laugh about bugs, it's not a beta version" << endl;
}

#endif
