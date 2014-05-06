#ifndef TURN_INCLUDED

#define TURN_INCLUDED

#include "game.cpp"
#include "save.cpp"

void finish_game(Labyrinth &map);
void erase_player_from_cell(Labyrinth &map, Player &p);
void insert_player_to_cell(Labyrinth &map, Player &p, Point to);
void kill_player(Labyrinth &map, Player &p);
bool use_bullet(Labyrinth &map, Player &p);
bool use_bomb(Labyrinth &map, Player &p);
void take_objects_from_cell(Labyrinth &map, Player &p);
int get_direction(Player &p);

bool leave_game(Labyrinth &map, Player &p) {
	kill_player(map, p);
	erase_player_from_cell(map, p);
	assert(p.in_game == true);
	p.in_game = false;
	map.players_in_game--;
	
	if (map.players_in_game == 0) {
		global_message("All players has left the game");
		exit(0);
	}
	return true;
}

bool bomb(Labyrinth &map, Player &p) {
	if (!use_bomb(map, p))
		return false;
	
	int dir = get_direction(p);
	int x = p.pos.x;
	int y = p.pos.y;
	Point to = p.pos + delta[dir];
	string message = p.name + " throwed the bomb " + dir_str[dir] + " and ";
	if (to.x < 0 || to.x >= map.size || to.y < 0 || to.y >= map.size ||
			!map.wall[dir][x][y]) {
		user_message(message + "nothing has happened");
		p.fails++;
		return true;
	}
	map.wall[dir][x][y] = false;
	map.wall[dir ^ 1][to.x][to.y] = false;
	p.good_turns++;
	user_message(message + "successfully destroyed the wall!");
	return true;
}

bool go(Labyrinth &map, Player &p) {
	int dir = get_direction(p);
	int x = p.pos.x;
	int y = p.pos.y;
	string message = p.name + " tried to move " + dir_str[dir] + " and ";
	if (map.wall[dir][x][y]) {
		user_message(message + "broke nose! It was a wall");
		return true;
	}
	
	Point to = p.pos + delta[dir];
	if (to.x < 0 || to.x >= map.size || to.y < 0 || to.y >= map.size) {
		if (p.treasures) {
			finish_game(map);
			return true;
		}
		user_message(message + "found the exit. Unfortunately, " + p.name + " can't leave the labyrinth without treasure");
		return true;
	}
	erase_player_from_cell(map, p);
	insert_player_to_cell(map, p, to);
	if (map.cell[to.x][to.y].type == MINOTAUR) {
		user_message(message + "have been eaten by the Minotaur! " + p.name +  " is in the mortuary now");
		kill_player(map, p);
		return true;
	}
	
	if (map.cell[to.x][to.y].type == PORTAL) {
		int portal = map.cell[to.x][to.y].portal_id;
		int new_portal = map.cell[to.x][to.y].portal_id + 1;
		if (new_portal == (int) map.portal.size())
			new_portal = 0;
		user_message(message + " entered the portal #" + to_string(portal + 1)
				+ " and appeared in the portal #" + to_string(new_portal + 1));
		erase_player_from_cell(map, p);
		to = map.portal[new_portal];
		insert_player_to_cell(map, p, to);
		take_objects_from_cell(map, p);
	} else {
		user_message(message + "moved successfully");
		take_objects_from_cell(map, p);
	}
	return true;
}

bool shoot(Labyrinth &map, Player &p) {
	if (!use_bullet(map, p))
		return false;
	int dir = get_direction(p);
	Point cur = p.pos;
	
	string message = p.name + " shooted " + dir_str[dir] + " and";
	while (true) {
		Point next = cur + delta[dir];
		if (map.wall[dir][cur.x][cur.y]) {
			user_message(message + " hit the wall");
			p.fails++;
			return true;
		}
		cur = next;
		if (cur.x < 0 || cur.x >= map.size || cur.y < 0 || cur.y >= map.size)
			break;
		if (map.cell[cur.x][cur.y].type == MINOTAUR) {
			map.cell[cur.x][cur.y].type = DEAD_MINOTAUR;
			p.good_turns++;
			user_message(message + "... Nooo! They killed the Minotaur. They're bastards!");
			return true;
		}
		if (map.cell[cur.x][cur.y].type == DEAD_MINOTAUR) {
			user_message(message + " hit the skin of Minotaur");
			return true;
		}
		if (!map.cell[cur.x][cur.y].player.empty()) {
			int x = rand() % int(map.cell[cur.x][cur.y].player.size());
			int killed = map.cell[cur.x][cur.y].player[x];
			p.good_turns++;
			user_message(message + " killed " + map.player[killed].name + "!");
			kill_player(map, map.player[killed]);
			return true;
		}
	}
	user_message(message + " the bullet has left the labyrinth");
	return true;
}

bool suicide(Labyrinth &map, Player &p) {
	if (!use_bullet(map, p))
		return false;
	kill_player(map, p);
	user_message(p.name + " successfully made suicide. Loser!");
	return true;
}

bool use_knife(Labyrinth &map, Player &p) {
	int x = p.pos.x;
	int y = p.pos.y;
	
	assert(map.cell[x][y].player.size() >= 1);
	string message = p.name + " waved a knife and ";
	if (map.cell[x][y].player.size() == 1) {
		user_message(message + "nothing has happened");
		p.fails++;
		return true;
	}
	vector <int> losers = map.cell[x][y].player;
	for (int i = 0; i < (int) losers.size(); ++i) {
		int killed = losers[i];
		if (killed == p.id)
			continue;
		kill_player(map, map.player[killed]);
		p.good_turns++;
		user_message(message + " successfully killed " + map.player[killed].name);
	}
	take_objects_from_cell(map, p);
	return true;
}

bool stay(Player &p) {
	p.fails++;
	user_message(p.name + "stayed on his place");
	return true;
}

bool save_game(Labyrinth &map) {
	if (!SERVER) {
		user_message("Enter file name");
		string name;
		cin >> name;
		name += ".lab";
		FILE *out = fopen(name.c_str(), "wb");
		if (!out) {
			user_message("Failed to save game to file " + name);
			return false;
		}
		print(map, out);
		fclose(out);
		user_message("Game successfully saved to file " + name);
		return true;
	} else {
		user_message("You can't save game in server mode");
		return false;
	}
}

#endif
