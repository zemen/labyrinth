#ifndef GAME_INCLUDED

#define GAME_INCLUDED

#include "header.cpp"
#include "debug.cpp"
#include "turn.cpp"

// OMG, dont't read this!
bool is_developer(Player &p) {
	return p.name == "zemen" || p.name == "pershik" || p.name == "matushkin";
}

void finish_game(Labyrinth &map) {
	int winner = map.current_player;
	cout << endl;
	cout << "Player " << map.player[winner].name << " has won the game!" << endl;
	for (int i = 0; i < (int) map.player.size(); ++i) {
		cout << map.player[i].name << " has been killed " << map.player[i].deaths << " times" << endl;
		cout << map.player[i].name << " has failed " << map.player[i].fails << " times" << endl;
		cout << map.player[i].name << " has made " << map.player[i].good_turns << " good turns" << endl;
	}
	
	int worst = __INT_MAX__;
	for (int i = 0; i < (int) map.player.size(); ++i) {
		if (i == winner || is_developer(map.player[i]))
			continue;
		worst = min(worst, 5 * map.player[i].good_turns - map.player[i].fails);
	}
	vector <int> losers(0);
	for (int i = 0; i < (int) map.player.size(); ++i) {
		if (i == winner || is_developer(map.player[i]))
			continue;
		if (5 * map.player[i].good_turns - map.player[i].fails > worst)
			continue;
		losers.push_back(i);
	}
	
	cout << "The current labyrinth state is:" << endl;
	print_labyrinth(map);
	if (!losers.empty()) {
		int loser = losers[rand() % int(losers.size())];
		cout << map.player[loser].name << " must give a big chocolate to "
				<< map.player[winner].name << endl;
	}
	exit(0);
}

void erase_player_from_cell(Labyrinth &map, Player &p) {
	int x = p.pos.x;
	int y = p.pos.y;
	for (int i = 0; i < (int) map.cell[x][y].player.size(); ++i)
		if (map.cell[x][y].player[i] == p.id) {
			map.cell[x][y].player.erase(map.cell[x][y].player.begin() + i);
			break;
		}
}

void insert_player_to_cell(Labyrinth &map, Player &p, Point to) {
	map.cell[to.x][to.y].player.push_back(p.id);
	p.pos = to;
}

bool use_bullet(Labyrinth &map, Player &p) {
	if (p.bullets == 0) {
		cout << "You're cheater! You haven't enough bullets" << endl;
		return false;
	}
	p.bullets--;
	map.bullets_in_game--;
	return true;
}

bool use_bomb(Labyrinth &map, Player &p) {
	if (p.bombs == 0) {
		cout << "You're cheater! You haven't enough bombs" << endl;
		return false;
	}
	p.bombs--;
	map.bombs_in_game--;
	return true;
}

void kill_player(Labyrinth &map, Player &p) {
	int x = p.pos.x;
	int y = p.pos.y;
	map.cell[x][y].bullets += p.bullets;
	map.cell[x][y].bombs += p.bombs;
	map.cell[x][y].treasures += p.treasures;
	map.bombs_in_game -= p.bombs;
	map.bullets_in_game -= p.bullets;
	p.bombs = p.bullets = p.treasures = 0;
	p.deaths++;
	p.fails++;
	erase_player_from_cell(map, p);
	insert_player_to_cell(map, p, map.mortuary);
}

string get_idiot_phraze(Player &p) {
	if (is_developer(p))
		return "Sorry, Master, but it's incorrect! You can enter correct information, if you want";
	return idiot_string[rand() % int(idiot_string.size())];
}

void take_objects_from_cell(Labyrinth &map, Player &p) {
	int x = p.pos.x;
	int y = p.pos.y;
		
	p.bullets += map.cell[x][y].bullets;
	p.bombs += map.cell[x][y].bombs;
	p.treasures += map.cell[x][y].treasures;
	map.bombs_in_game += map.cell[x][y].bombs;
	map.bullets_in_game += map.cell[x][y].bullets;
	
	if (map.cell[x][y].treasures) {
		p.good_turns++;
		cout << "You have found the treasure!" << endl;
	}
	
	if (map.cell[x][y].bullets == 1)
		cout << "You have taken one bullet" << endl;
	else if (map.cell[x][y].bullets > 1)
		cout << "You have taken " << map.cell[x][y].bullets << " bullets" << endl;
	
	if (map.cell[x][y].bombs == 1)
		cout << "You have taken one bomb" << endl;
	else if (map.cell[x][y].bombs > 1)
		cout << "You have taken " << map.cell[x][y].bombs << " bombs" << endl;
	
	map.cell[x][y].bullets = 0;
	map.cell[x][y].bombs = 0;
	map.cell[x][y].treasures = 0;
}

int get_direction(Player &p) {
	cout << "Enter direction (u, d, l, r)" << endl;
	string dir;
	cin >> dir;
	while (dir != "u" && dir != "d" && dir != "l" && dir != "r") {
		cout << get_idiot_phraze(p)
				<< " The direction is incorrect. Try again" << endl;
		cin >> dir;
		p.fails++;
	}
	int to;
	if (dir == "r")
		to = RIGHT;
	else if (dir == "l")
		to = LEFT;
	else if (dir == "u")
		to = UP;
	else if (dir == "d")
		to = DOWN;
	else
		assert(false);
	return to;
}

struct EndOfTurn {
	EndOfTurn() {}
	
	~EndOfTurn() {
		string trash;
		getline(cin, trash);
	}
};

bool leave_game(Labyrinth &, Player &);
bool suicide(Labyrinth &, Player &);
bool use_knife(Labyrinth &, Player &);
bool bomb(Labyrinth &, Player &);
bool go(Labyrinth &, Player &);
bool shoot(Labyrinth &, Player &);
bool stay(Player &);

bool turn(Labyrinth &map) {
	take_objects_from_cell(map, map.player[map.current_player]);
	EndOfTurn anticheat; // Read the whole line after return to protect the next player
	if (DEBUG)
		print_debug(map);
	cout << endl;
	cout << "Player " << map.player[map.current_player].name << ", your turn" << endl;
	cout << "Enter what you want (go, bomb, shoot, knife, suicide, stay, leave)" << endl;
	string s;
	cin >> s;
	
	if (s == "leave")
		return leave_game(map, map.player[map.current_player]);
	if (s == "suicide")
		return suicide(map, map.player[map.current_player]);
	if (s == "knife")
		return use_knife(map, map.player[map.current_player]);
	if (s == "bomb")
		return bomb(map, map.player[map.current_player]);
	if (s == "go")
		return go(map, map.player[map.current_player]);
	if (s == "shoot")
		return shoot(map, map.player[map.current_player]);
	if (s == "stay")
		return stay(map.player[map.current_player]);
	if (s == "winthemall") {
		if (is_developer(map.player[map.current_player])) {
			cout << "Okay, master" << endl;
			finish_game(map);
			return true;
		}
	}
	
	cout << "Incorrect! Try again" << endl;
	return false;
}

void god_turn(Labyrinth &map) {
	if (map.bombs_in_game == 0) {
		cout << "Everybody gets one more bomb!" << endl;
		for (int i = 0; i < (int) map.player.size(); ++i) {
			if (!map.player[i].in_game)
				continue;
			map.player[i].bombs++;
		}
		map.bombs_in_game = map.players_in_game;
	}
	
	if (map.bullets_in_game == 0) {
		cout << "Everybody gets one more bullet!" << endl;
		for (int i = 0; i < (int) map.player.size(); ++i) {
			if (!map.player[i].in_game)
				continue;
			map.player[i].bullets++;
		}
		map.bullets_in_game = map.players_in_game;
	}
}

void play(Labyrinth &map) {
	while (true) {
		if (map.player[map.current_player].in_game) {
			while (!turn(map));
			god_turn(map);
		}
		if (DEBUG)
			check_labyrinth(map);
		++map.current_player;
		if (map.current_player == int(map.player.size()))
			map.current_player = 0;
	}
}

#endif
