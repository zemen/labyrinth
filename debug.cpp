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

#endif
