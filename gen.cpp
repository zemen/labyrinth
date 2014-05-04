#ifndef GEN_INCLUDED

#define GEN_INCLUDED

#include "header.cpp"
#include "debug.cpp"

//~ DSU for Kruskal
struct DSU {
	vector <int> col;
	vector <int> rk;
	
	void init(int n) {
		col.clear();
		rk.clear();
		rk.resize(n, 0);
		col.resize(n);
		for (int i = 0; i < n; ++i)
			col[i] = i;
	}
	
	int get(int u) {
		if (col[u] == u)
			return u;
		return col[u] = get(col[u]);
	}
	
	bool join(int u, int v) {
		u = get(u);
		v = get(v);
		if (u == v)
			return false;
		if (rk[u] > rk[v])
			swap(u, v);
		if (rk[u] == rk[v])
			++rk[v];
		col[u] = v;
		return true;
	}
};

void generate_walls(Labyrinth &map) {
	//~ Simple Kruskal algorithm with random edge weights
	DSU s;
	s.init(map.size * map.size);
	vector <pair <Point, int> > edge(0);
	for (int x = 0; x < map.size - 1; ++x)
		for (int y = 0; y < map.size; ++y)
			edge.push_back(make_pair(Point(x, y), DOWN));
	for (int x = 0; x < map.size; ++x)
		for (int y = 0; y < map.size - 1; ++y)
			edge.push_back(make_pair(Point(x, y), RIGHT));
	random_shuffle(edge.begin(), edge.end());
	int remaining = map.size * map.size - 1;
	while (remaining) {
		Point p = edge.back().first;
		int dir = edge.back().second;
		edge.pop_back();
		Point to = p + delta[dir];
		if (s.join(p.x * map.size + p.y, to.x * map.size + to.y)) {
			--remaining;
			map.wall[dir][p.x][p.y] = false;
			map.wall[dir ^ 1][to.x][to.y] = false;
		}
	}
}

Point get_point(vector <Point> &v) {
	Point p = v.back();
	v.pop_back();
	return p;
}

Labyrinth generate_labyrinth(Parameters params) {
	Labyrinth map;
	map.size = params.size;
	map.current_player = 0;
	map.players_in_game = params.players;
	map.bombs_in_game = params.bombs * params.players;
	map.bullets_in_game = params.bullets * params.players;
	map.cell.resize(map.size, vector <Cell>(map.size, Cell()));
	if (DEBUG) {
		cerr << "Generating walls ";
		cerr.flush();
	}
	for (int dir = 0; dir < 4; ++dir)
		map.wall[dir].resize(map.size, vector <bool>(map.size, true));
	generate_walls(map);
	if (DEBUG)
		cerr << "[ok]" << endl;
	
	if (DEBUG) {
		cerr << "Generating objects ";
		cerr.flush();
	}
	vector <Point> empty;
	for (int x = 0; x < map.size; ++x)
		for (int y = 0; y < map.size; ++y)
			empty.push_back(Point(x, y));
	random_shuffle(empty.begin(), empty.end());
	Point p;
	map.mortuary = p = get_point(empty);
	map.cell[p.x][p.y].type = MORTUARY;
	for (int i = 0; i < params.minotaurs; ++i) {
		p = get_point(empty);
		map.cell[p.x][p.y].type = MINOTAUR;
		if (i == 0)
			map.cell[p.x][p.y].treasures = 1;
	}
	for (int i = 0; i < params.players; ++i) {
		p = get_point(empty);
		map.player.push_back(Player(i, params.player_names[i], p, params.bullets, params.bombs));
		map.cell[p.x][p.y].player.push_back(i);
	}
	for (int i = 0; i < params.portals; ++i) {
		p = get_point(empty);
		map.portal.push_back(p);
		map.cell[p.x][p.y].type = PORTAL;
		map.cell[p.x][p.y].portal_id = i;
	}
	
	vector <Point> exit; // Not really Point, just pair of direction and coordinate
	for (int dir = 0; dir < 4; ++dir)
		for (int i = 0; i < map.size; ++i)
			exit.push_back(Point(dir, i));
	random_shuffle(exit.begin(), exit.end());
	for (int i = 0; i < params.exits; ++i) {
		p = get_point(exit);
		if (p.x == UP)
			map.wall[UP][0][p.y] = false;
		else if (p.x == DOWN)
			map.wall[DOWN][map.size - 1][p.y] = false;
		else if (p.x == LEFT)
			map.wall[LEFT][p.y][0] = false;
		else if (p.x == RIGHT)
			map.wall[RIGHT][p.y][map.size - 1] = false;
	}
	int edges = 0;
	for (int x = 0; x < map.size; ++x)
		for (int y = 0; y < map.size; ++y)
			for (int dir = 0; dir < 4; ++dir)
				edges += (map.wall[dir][x][y] == 0);
	if (DEBUG)
		cerr << "[ok]" << endl;
	if (DEBUG)
		check_labyrinth(map);
	return map;
}

#endif
