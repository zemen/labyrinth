#ifndef SAVE_INCLUDED

#define SAVE_INCLUDED

#include "header.cpp"

void print(char c, FILE *out) {
	fwrite(&c, sizeof(char), 1, out);
}

void print(bool b, FILE *out) {
	print(char(b), out);
}

void print(int x, FILE *out) {
	fwrite(&x, sizeof(int), 1, out);
}

void print(string s, FILE *out) {
	print(int(s.length()), out);
	for (int i = 0; i < (int) s.size(); ++i)
		print(char(s[i]), out);
}

template <typename T>
void print(vector <T> v, FILE *out) {
	print(int(v.size()), out);
	for (int i = 0; i < (int) v.size(); ++i)
		print(v[i], out);
}

void print(Point p, FILE *out) {
	print(p.x, out);
	print(p.y, out);
}

void print(Player p, FILE *out) {
	print(p.id, out);
	print(p.name, out);
	print(p.in_game, out);
	print(p.bullets, out);
	print(p.bombs, out);
	print(p.treasures, out);
	print(p.fails, out);
	print(p.good_turns, out);
	print(p.deaths, out);
	print(p.pos, out);
}

void print(Cell &p, FILE *out) {
	print(p.bullets, out);
	print(p.bombs, out);
	print(p.treasures, out);
	print(p.type, out);
	print(p.portal_id, out);
	print(p.player, out);
}

void print(Labyrinth &p, FILE *out) {
	print(string("labyrinth"), out);
	print(p.size, out);
	print(p.current_player, out);
	print(p.players_in_game, out);
	print(p.bombs_in_game, out);
	print(p.bullets_in_game, out);
	print(p.cell, out);
	print(p.portal, out);
	print(p.player, out);
	for (int i = 0; i < 4; ++i)
		print(p.wall[i], out);
	print(p.mortuary, out);
	print(string("end"), out);
}

void read(char &c, FILE *in) {
	fread(&c, sizeof(char), 1, in);
}

void read(bool &b, FILE *in) {
	char c;
	read(c, in);
	b = bool(c);
}

bool read_bool(FILE *in) {
	char c;
	read(c, in);
	return bool(c);
}

void read(int &x, FILE *in) {
	fread(&x, sizeof(int), 1, in);
}

void read(string &s, FILE *in) {
	int len;
	read(len, in);
	s.resize(len);
	for (int i = 0; i < len; ++i)
		read(s[i], in);
}

template <typename T>
void read(vector <T> &v, FILE *in) {
	int len;
	read(len, in);
	v.resize(len);
	for (int i = 0; i < len; ++i)
		read(v[i], in);
}

void read(vector <bool> &v, FILE *in) {
	int len;
	read(len, in);
	v.resize(len);
	for (int i = 0; i < len; ++i)
		v[i] = read_bool(in);
}

void read(Point &p, FILE *in) {
	read(p.x, in);
	read(p.y, in);
}

void read(Player &p, FILE *in) {
	read(p.id, in);
	read(p.name, in);
	read(p.in_game, in);
	read(p.bullets, in);
	read(p.bombs, in);
	read(p.treasures, in);
	read(p.fails, in);
	read(p.good_turns, in);
	read(p.deaths, in);
	read(p.pos, in);
}

void read(Cell &p, FILE *in) {
	read(p.bullets, in);
	read(p.bombs, in);
	read(p.treasures, in);
	read(p.type, in);
	read(p.portal_id, in);
	read(p.player, in);
}

bool read(Labyrinth &p, FILE *in) {
	string s;
	read(s, in);
	if (s != "labyrinth")
		return false;
	read(p.size, in);
	read(p.current_player, in);
	read(p.players_in_game, in);
	read(p.bombs_in_game, in);
	read(p.bullets_in_game, in);
	read(p.cell, in);
	read(p.portal, in);
	read(p.player, in);
	for (int i = 0; i < 4; ++i)
		read(p.wall[i], in);
	read(p.mortuary, in);
	read(s, in);
	if (s != "end")
		return false;
	return true;
}

#endif
