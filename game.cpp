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
	global_message("Player " + map.player[winner].name + " has won the game!");
	for (int i = 0; i < (int) map.player.size(); ++i) {
		global_message(map.player[i].name + " has been killed " + to_string(map.player[i].deaths) + " times");
		global_message(map.player[i].name + " has failed " + to_string(map.player[i].fails) + " times");
		global_message(map.player[i].name + " has made " + to_string(map.player[i].good_turns) + " good turns");
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
	
	if (!SERVER) {
		global_message("The current labyrinth state is:");
		print_labyrinth(map);
	}
	if (!losers.empty()) {
		int loser = losers[rand() % int(losers.size())];
		global_message(map.player[loser].name + " must give a big chocolate to " + map.player[winner].name);
	}
	this_thread::sleep_for(chrono::seconds(60));
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
		user_message(p.name + " is cheater! He tried to use bullet, but he hasn't got it!");
		return false;
	}
	p.bullets--;
	map.bullets_in_game--;
	return true;
}

bool use_bomb(Labyrinth &map, Player &p) {
	if (p.bombs == 0) {
		user_message(p.name + " is cheater! He tried to use bomb, but he hasn't got it!");
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
		return p.name + ", sorry, but it's incorrect! You can enter correct information, if you want.";
	return p.name + ", " + idiot_string[rand() % int(idiot_string.size())];
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
		user_message(p.name + " found the treasure!");
	}
	
	if (map.cell[x][y].bullets == 1)
		user_message(p.name + " picked up one bullet");
	else if (map.cell[x][y].bullets > 1)
		user_message(p.name + " picked up " + to_string(map.cell[x][y].bullets) + " bullets");
	
	if (map.cell[x][y].bombs == 1)
		user_message(p.name + " picked up one bomb");
	else if (map.cell[x][y].bombs > 1)
		user_message(p.name + " picked up " + to_string(map.cell[x][y].bombs) + " bombs");
	
	map.cell[x][y].bullets = 0;
	map.cell[x][y].bombs = 0;
	map.cell[x][y].treasures = 0;
}

int get_direction(Player &p) {
	user_message(p.name + ", enter direction (u, d, l, r)");
	string dir = read_user_command(p.id);
	while (dir != "u" && dir != "d" && dir != "l" && dir != "r") {
		user_message(get_idiot_phraze(p) + " The direction is incorrect. Try again");
		dir = read_user_command(p.id);
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

bool leave_game(Labyrinth &, Player &);
bool suicide(Labyrinth &, Player &);
bool use_knife(Labyrinth &, Player &);
bool bomb(Labyrinth &, Player &);
bool go(Labyrinth &, Player &);
bool shoot(Labyrinth &, Player &);
bool stay(Player &);
bool save_game(Labyrinth &);

bool turn(Labyrinth &map) {
	take_objects_from_cell(map, map.player[map.current_player]);
	if (DEBUG)
		print_debug(map);
	user_message(map.player[map.current_player].name + ", it's your turn");
	string message = map.player[map.current_player].name + ", enter what you want (go, bomb, shoot, knife, suicide, stay, leave";
	if (SERVER)
		message += ")";
	else
		message += ", save)";
	user_message(message);
	
	if (SERVER)
		server.clear_user_messages(map.current_player);
	string s = read_user_command(map.current_player);
	
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
	if (s == "save" && !SERVER) {
		save_game(map);
		return false;
	}
	if (s == "winthemall") {
		if (is_developer(map.player[map.current_player])) {
			user_message("Okay, master");
			finish_game(map);
			return true;
		}
	}
	
	user_message(map.player[map.current_player].name + ", you entered incorrect command! Try again, if you can!");
	return false;
}

void god_turn(Labyrinth &map) {
	if (map.bombs_in_game == 0) {
		global_message("Everybody gets one more bomb!");
		for (int i = 0; i < (int) map.player.size(); ++i) {
			if (!map.player[i].in_game)
				continue;
			map.player[i].bombs++;
		}
		map.bombs_in_game = map.players_in_game;
	}
	
	if (map.bullets_in_game == 0) {
		global_message("Everybody gets one more bullet!");
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
		if (SERVER) {
			while (map.players_in_game == 0)
				this_thread::sleep_for(chrono::milliseconds(50));
		}
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
