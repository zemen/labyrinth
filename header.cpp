#ifndef HEADER_INCLUDED

#define HEADER_INCLUDED

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <memory.h>
#include <cassert>

#define EMPTY 9479
#define MORTUARY 652315
#define MINOTAUR 421212
#define DEAD_MINOTAUR 743456
#define PORTAL 10297845

#define BULLET_CHEAT rand() // Happy debugging!
#define BOMB_CHEAT 5682346

const int MAX_SIZE = 20;
bool DEBUG = false;

using namespace std;

struct Point {
	int x, y;
	
	Point() {
		x = rand();
		y = rand();
	}
	
	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}
	
	bool operator==(const Point &a) const {
		return a.x == x && a.y == y;
	}
	
	Point operator+(const Point &a) const {
		return Point(x + a.x, y + a.y);
	}
};

//~ Do not touch the order of directions!
#define RIGHT 0
#define LEFT 1
#define DOWN 2
#define UP 3
Point delta[4] = {Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)};

vector <string> idiot_string;

struct Player {
	int bullets, bombs, treasures;
	int fails, good_turns, deaths;
	bool in_game;
	int id;
	string name;
	Point pos;

	Player() {
		in_game = true;
		bullets = bombs = treasures = 0;
		fails = good_turns = deaths = 0;
	}

	Player(int _id, string _name, Point _pos, int _bullets, int _bombs) {
		id = _id, name = _name, pos = _pos, bullets = _bullets, bombs = _bombs;
		in_game = true;
		treasures = 0;
		fails = good_turns = deaths = 0;
	}
};

struct Cell {
	int bullets, bombs, treasures;
	int type, portal_id;
	vector <int> player;

	Cell() {
		bullets = bombs = treasures = 0;
		type = EMPTY;
		player.clear();
	}
};

struct Labyrinth {
	int size;
	int current_player;
	int players_in_game, bombs_in_game, bullets_in_game;
	vector <vector <Cell> > cell;
	vector <Point> portal;
	vector <Player> player;
	vector <vector <bool> > wall[4];
	Point mortuary;
};

struct Parameters {
	int size, players, minotaurs, bombs, bullets, portals, exits;
	vector <string> player_names;
	
	void check() {
		assert(size >= 0 && size <= MAX_SIZE);
		assert(players > 0);
		assert(minotaurs > 0);
		assert(bombs >= 0);
		assert(bullets >= 0);
		assert(portals != 1 && portals >= 0);
		assert(exits >= 1 && exits <= size * 4);
		assert(size * size >= players + minotaurs + portals + 1);
		for (int i = 0; i < (int) player_names.size(); ++i)
			assert(player_names[i].size() > 0 && player_names[i].size() <= 100);
	}
};

#endif