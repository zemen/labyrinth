#ifndef TURN_INCLUDED

#define TURN_INCLUDED

#include "game.cpp"

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
		cout << "All players has left the game" << endl;
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
	if (to.x < 0 || to.x >= map.size || to.y < 0 || to.y >= map.size ||
			!map.wall[dir][x][y]) {
		cout << "Nothing has happened" << endl;
		p.fails++;
		return true;
	}
	map.wall[dir][x][y] = false;
	map.wall[dir ^ 1][to.x][to.y] = false;
	p.good_turns++;
	cout << "You have successfully destroyed the wall!" << endl;
	return true;
}

bool go(Labyrinth &map, Player &p) {
	int dir = get_direction(p);
	int x = p.pos.x;
	int y = p.pos.y;
	if (map.wall[dir][x][y]) {
		cout << "You've broken your nose! It's a wall" << endl;
		return true;
	}
	
	Point to = p.pos + delta[dir];
	if (to.x < 0 || to.x >= map.size || to.y < 0 || to.y >= map.size) {
		if (p.treasures) {
			finish_game(map);
			return true;
		}
		cout << "You have found the exit but can't leave the labyrinth without treasure" << endl;
		return true;
	}
	erase_player_from_cell(map, p);
	insert_player_to_cell(map, p, to);
	if (map.cell[to.x][to.y].type == MINOTAUR) {
		cout << "You're eaten by the Minotaur! You are in the mortuary now" << endl;
		kill_player(map, p);
		return true;
	}
	
	if (map.cell[to.x][to.y].type == PORTAL) {
		int portal = map.cell[to.x][to.y].portal_id;
		int new_portal = map.cell[to.x][to.y].portal_id + 1;
		if (new_portal == (int) map.portal.size())
			new_portal = 0;
		cout << "You've entered the portal #" << portal + 1
				<< " and now you're in the portal #" << new_portal + 1
				<< endl;
		erase_player_from_cell(map, p);
		to = map.portal[new_portal];
		insert_player_to_cell(map, p, to);
		take_objects_from_cell(map, p);
	} else {
		cout << "Success!" << endl;
		take_objects_from_cell(map, p);
	}
	return true;
}

bool shoot(Labyrinth &map, Player &p) {
	if (!use_bullet(map, p))
		return false;
	int dir = get_direction(p);
	Point cur = p.pos;
	
	while (true) {
		Point next = cur + delta[dir];
		if (map.wall[dir][cur.x][cur.y]) {
			cout << "The bullet is in the wall" << endl;
			p.fails++;
			return true;
		}
		cur = next;
		if (cur.x < 0 || cur.x >= map.size || cur.y < 0 || cur.y >= map.size)
			break;
		if (map.cell[cur.x][cur.y].type == MINOTAUR) {
			map.cell[cur.x][cur.y].type = DEAD_MINOTAUR;
			p.good_turns++;
			cout << "They killed the Minotaur. They're bastards!" << endl;
			return true;
		}
		if (map.cell[cur.x][cur.y].type == DEAD_MINOTAUR) {
			cout << "You've made skin of Minotaur bad!" << endl;
			return true;
		}
		if (!map.cell[cur.x][cur.y].player.empty()) {
			int x = rand() % int(map.cell[cur.x][cur.y].player.size());
			int killed = map.cell[cur.x][cur.y].player[x];
			p.good_turns++;
			cout << "You've killed " << map.player[killed].name << "!" << endl;
			kill_player(map, map.player[killed]);
			return true;
		}
	}
	cout << "The bullet has left the labyrinth" << endl;
	return true;
}

bool suicide(Labyrinth &map, Player &p) {
	if (!use_bullet(map, p))
		return false;
	kill_player(map, p);
	cout << "You have successfully made suicide, loser" << endl;
	return true;
}

bool use_knife(Labyrinth &map, Player &p) {
	int x = p.pos.x;
	int y = p.pos.y;
	
	assert(map.cell[x][y].player.size() >= 1);
	if (map.cell[x][y].player.size() == 1) {
		cout << "It's a fail!" << endl;
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
		cout << "You have successfully killed " << map.player[killed].name << endl;
	}
	take_objects_from_cell(map, p);
	return true;
}

bool stay(Player &p) {
	p.fails++;
	cout << "Okay" << endl;
	return true;
}

#endif
