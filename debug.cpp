#ifndef DEBUG_INCLUDED

#define DEBUG_INCLUDED

#include "header.cpp"

// A piece of shit
void print_labyrinth(Labyrinth &map, ostream &out = cout) {
	int x, y;
	int n = map.size;
	for (int i = 0; i <= 2 * n; i++) {
		for (int j = 0; j <= 2 * n; j++) {
			
			if (i % 2 == 0 && j % 2 == 0) {
				out << "#";
				continue;
			}
			
			if (i == 0 || i == 2 * n || j == 0 || j == 2 * n) {
				bool is_wall = false;
				if (i == 2 * n)
					is_wall = map.wall[DOWN][n - 1][j / 2];
				else if (i == 0)
					is_wall = map.wall[UP][0][j / 2];
				else if (j == 2 * n)
					is_wall = map.wall[RIGHT][i / 2][n - 1];
				else if (j == 0)
					is_wall = map.wall[LEFT][i / 2][0];
				if (is_wall)
					out << "#";
				else
					out << " ";
				continue;
			}
			
			if (i % 2 == 1 && j % 2 == 1) {
				x = i / 2;
				y = j / 2;
				if (map.cell[x][y].type == MINOTAUR)
					out << "M";
				else if (map.cell[x][y].type == DEAD_MINOTAUR)
					out << "D";
				else if (map.cell[x][y].type == MORTUARY)
					out << "m";
				else if (map.cell[x][y].type == PORTAL)
					out << "P";
				else
					out << "_";
			}
			
			if (i % 2 == 0 && j % 2 == 1) {
				x = i / 2 - 1;
				y = j / 2;
				if (map.wall[DOWN][x][y])
					out << "#";
				else
					out << " ";
			}
			
			if (i % 2 == 1 && j % 2 == 0) {
				x = i / 2;
				y = j / 2 - 1;
				if (map.wall[RIGHT][x][y])
					out << "#";
				else
					out << " ";
			}
		}
		out << endl;
	}
}

void print_debug(Labyrinth &map) {
	cerr << "DEBUG INFO" << endl;
	cerr << "Players:" << endl;
	for (int i = 0; i < (int) map.player.size(); i++) {
		cerr << map.player[i].name << endl;
		cerr << "X: " << map.player[i].pos.x << " Y: " << map.player[i].pos.y << " Bombs: "
				<< map.player[i].bombs << " Bullets: " << map.player[i].bullets << endl;
	}
	if (!map.portal.empty())
		cerr << "Portals: " << endl;
	for (int i = 0; i < (int) map.portal.size(); ++i) {
		cerr << i + 1 << ": " << map.portal[i].x << " " << map.portal[i].y << endl;
	}
	print_labyrinth(map, cerr);
}

void check_point(Point p, int n) {
	assert(p.x >= 0 && p.x < n && p.y >= 0 && p.y < n);
}

void check_labyrinth(Labyrinth &map) {
	assert((int) map.cell.size() == map.size);
	for (int i = 0; i < map.size; ++i)
		assert((int) map.cell[i].size() == map.size);
	assert(map.current_player >= 0);
	if (!SERVER)
		assert(map.current_player < (int) map.player.size());
	int players = 0;
	int bombs = 0, bullets = 0, treasures = 0;
	check_point(map.mortuary, map.size);
	assert(map.cell[map.mortuary.x][map.mortuary.y].type == MORTUARY);
	set <string> names;
	for (int i = 0; i < (int) map.player.size(); ++i) {
		players += map.player[i].in_game;
		if (!map.player[i].in_game) {
			assert(map.player[i].bombs == 0);
			assert(map.player[i].bullets == 0);
			assert(map.player[i].treasures == 0);
		} else {
			bombs += map.player[i].bombs;
			bullets += map.player[i].bullets;
			treasures += map.player[i].treasures;
		}
		assert(map.player[i].id == i);
		assert(map.player[i].name.size() > 0 && map.player[i].name.size() <= 100);
		names.insert(map.player[i].name);
		check_point(map.player[i].pos, map.size);
	}
	assert((int) names.size() == (int) map.player.size());
	assert(players == map.players_in_game);
	assert(bombs == map.bombs_in_game);
	assert(bullets == map.bullets_in_game);
	assert(treasures <= 1);
	for (int i = 0; i < (int) map.portal.size(); ++i) {
		check_point(map.portal[i], map.size);
		assert(map.cell[map.portal[i].x][map.portal[i].y].portal_id == i);
	}
	for (int x = 0; x < map.size; ++x)
		for (int y = 0; y < map.size - 1; ++y)
			assert(map.wall[RIGHT][x][y] == map.wall[LEFT][x][y + 1]);
	for (int x = 0; x < map.size - 1; ++x)
		for (int y = 0; y < map.size; ++y)
			assert(map.wall[DOWN][x][y] == map.wall[UP][x + 1][y]);
	players = 0;
	for (int x = 0; x < map.size; ++x)
		for (int y = 0; y < map.size; ++y) {
			if (map.cell[x][y].type == MINOTAUR)
				assert(map.cell[x][y].player.empty());
			players += (int) map.cell[x][y].player.size();
			for (int i = 0; i < (int) map.cell[x][y].player.size(); ++i) {
				int p = map.cell[x][y].player[i];
				assert(map.player[p].pos == Point(x, y));
			}
		}
	assert(players == map.players_in_game);
}

#endif
