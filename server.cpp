#ifndef SERVER_INCLUDED

#define SERVER_INCLUDED

#include "header.cpp"
#include <map>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

const string config = "htdocs.conf";

struct Server {
	int game_id;
	int message_count;
	map <string, string> dict;
	string user_list_file;
	string messages_file;
	string info_file;
	string client_files_dir;
	bool started;
	
	int user_list_size;
	
	Server() {
		started = false;
		message_count = 0;
	}
	
	~Server() {
		if (started) {
			FILE *info = fopen(info_file.c_str(), "w");
			fclose(info);
		}
	}
	
	void init_server() {
		started = true;
		parse_config();
		game_id = abs(rand());
		
		FILE *user_list = fopen(user_list_file.c_str(), "w");
		fclose(user_list);
		
		FILE *info = fopen(info_file.c_str(), "w");
		fprintf(info, "%d\n", game_id);
		fclose(info);
		
		FILE *messages = fopen(messages_file.c_str(), "w");
		fclose(messages);
		
		user_list_size = get_file_size(user_list_file);
	}
	
	//~ It's a fucking bicycle!
	//~ Parse a config-like file
	#define READ_KEY 0
	#define READ_VALUE 1
	#define READ_COMMENT 2
	void parse_config() {
		FILE * conf = fopen(config.c_str(), "r");
		while (!feof(conf)) {
			string key = "";
			string value = "";
			int state = READ_KEY;
			bool quotes_opened = false;
			char c;
			while (fscanf(conf, "%c", &c) == 1) {
				if (c == '\n')
					break;
				if (state == READ_KEY) {
					if (c == '=')
						state = READ_VALUE;
					else if (c == '#')
						state = READ_COMMENT;
					else if (islower(c) || isupper(c) || isdigit(c) || c == '_')
						key += c;
				} else if (state == READ_VALUE) {
					if (c == '"')
						quotes_opened ^= 1;
					else if (c == '#' && !quotes_opened)
						state = READ_COMMENT;
					else if (quotes_opened)
						value += c;
				}
			}
			if (!key.empty() && !value.empty() && !quotes_opened)
				dict[key] = value;
		}
		user_list_file = dict["user_list_file"];
		info_file = dict["info_file"];
		messages_file = dict["messages_file"];
		client_files_dir = dict["client_files_dir"];
		fclose(conf);
	}
	
	int get_file_size(std::string filename) {
		FILE *file = fopen(filename.c_str(), "rb");
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		fclose(file);
		return size;
	}
	
	void clear_messages() {
		FILE *messages = fopen(messages_file.c_str(), "w");
		fclose(messages);
	}
	
	void delete_first_message() {
		--message_count;
		string out;
		FILE *messages = fopen(messages_file.c_str(), "r");
		char c;
		bool print = false;
		while (fscanf(messages, "%c", &c) == 1) {
			if (print)
				out += c;
			if (c == '\n')
				print = true;
		}
		fclose(messages);
		messages = fopen(messages_file.c_str(), "w");
		fputs(out.c_str(), messages);
		fclose(messages);
	}
	
	void print_message(string message) {
		++message_count;
		if (message_count > 10)
			delete_first_message();
		FILE *messages = fopen(messages_file.c_str(), "a");
		fprintf(messages, "%s\n", message.c_str());
		fclose(messages);
	}
	
	string get_client_message_file(int id) {
		return client_files_dir + '/' + to_string(id);
	}
	
	void clear_user_messages(int id) {
		string file = get_client_message_file(id);
		FILE *f = fopen(file.c_str(), "w");
		fclose(f);
	}
	
	bool get_user_message(int id, string &s) {
		string file = get_client_message_file(id);
		ifstream in(file);
		if (!in)
			return false;
		getline(in, s);
		if (!s.empty()) {
			in.close();
			clear_user_messages(id);
			return true;
		}
		in.close();
		return false;
	}
	
	bool get_new_user(string &username) {
		if (get_file_size(user_list_file) != user_list_size) {
			ifstream in(user_list_file, fstream::in | fstream::binary);
			in.seekg(user_list_size, ios::beg);
			getline(in, username);
			user_list_size = get_file_size(user_list_file);
			return true;
		}
		return false;
	}
	
	string wait_for_user_message(int id) {
		string s;
		while (true) {
			if (get_user_message(id, s))
				return s;
			this_thread::sleep_for(chrono::milliseconds(50));
		}
	}
	
};

Server server;

void global_message(string s) {
	if (!SERVER)
		cout << s << endl;
	else {
		cerr << s << endl;
		server.print_message(s);
	}
}

//~ TODO
void user_message(string s) {
	global_message(s);
}

void clear_messages() {
	if (SERVER)
		server.clear_messages();
}

string read_user_command(int id) {
	string s;
	if (SERVER)
		s = server.wait_for_user_message(id);
	else
		getline(cin, s);
	string word;
	istringstream in(s);
	in >> word;
	return word;
}

void accept_new_users(Server *s, Labyrinth *map, Parameters *params) {
	string username;
	while (true) {
		if (s->get_new_user(username)) {
			global_message(username + " joined the game");
			int id = (int) map->player.size();
			s->clear_user_messages(id);
			map->add_online_player(username, *params);
		}
		this_thread::sleep_for(chrono::milliseconds(50));
	}
}

#endif
