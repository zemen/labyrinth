#ifndef SAVE_INCLUDED

#define SAVE_INCLUDED

#include "header.cpp"

void print_int(int x) {
    for (int i = 0; i < 4; ++i) {
        fprintf(output, "%c", x % 256);
        x >>= 8;
    }
}

void print_string(string s) {
    print_int(s.length());
    for (int i = 0; i < (int) s.length(); ++i)
        fprintf(output, "%c", s[i]);
}

int read_int() {
    int res = 0;
    for (int i = 0; i < 4; ++i) {
        char buf;
        fscanf(input, "%c", &buf);
        res <<= 8;
        res += buf;
    }
    return res;
}

char read_char() {
    char res;
    fscanf(input, "%c", &res);
    return res;
}

string read_string() {
    string res = "";
    int len = read_int();
    for (int i = 0; i < len; ++i)
        res += read_char();
    return res;
}

void save_game(string name) {
    output = fopen(name.c_str(), "w");
    print_int(n);
    
}

#endif
