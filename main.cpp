/* Labyrinth Game
 *
 * by pershik & matushkin & zemen
 *
 * Changelog:
 * 
 * v12
 * - Server/client mode
 * - Now player can't pass direction for command in the line, that contains command(will be fixed)
 * 
 * v11
 * - Ability to save game to file and load from file
 * 
 * v10
 * - Absolutely new structured code!
 * - Kruskal algorithm used in generating labyrinth
 * - O(n^2) memory for walls, not O(n^4)
 * - Standard labyrinth parameters
 * - More intelligent labyrinth parameters checking
 * - Bullets and bombs for each player adds after each move, not after each loop
 * - Makefile created
 * 
 * v9
 * - Email is deleted
 * - Code is refactored
 *
 * v8
 * - Added parameter DEBUG
 * - My name is pershik now, not perhik
 * - E-mail is changed
 * - The function cheater is modified
 * - Added possibility to leave the game
 * - Added some bugs
 * - I like it more
 *
 * v7
 * - Modified getting information from user
 * - Added portals
 * - Another segmentation fault bug fixed
 *
 * v6
 * - Fixed spelling mistakes
 * - Fixed some bugs
 * - Added goods for every player
 * - Created changelog (:
 *
 * v5
 * - Modified create_labyrinth method
 *
 * v4
 * - Added second exit in labyrinth
 * - Added new turn commands(stay)
 * - Modified input method
 * - Fixed segmentation fault bug
 *
 * v3
 * - Added new printlab function
 * - Added debug mode
 * - Formatted the source
 * - Added segmentation fault bug :(
 *
 * v2
 * - Rebuilt structure of code(from 700 to 400 strings)
 * - Added randomPhrazeIdiot function (:
 * - Fixed lots of bugs
 *
 * v1
 * - Created code (:
 * - Fixed lots of bugs
 */

#include "header.cpp"
#include "server.cpp"
#include "gen.cpp"
#include "init.cpp"
#include "turn.cpp"
#include "game.cpp"
#include "debug.cpp"
#include "save.cpp"

int main(int argc, char **argv) {
	Parameters params;
	Labyrinth map;
	init();
	bool loaded = false;
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--debug") == 0)
			DEBUG = true;
		else if (strcmp(argv[i], "--server") == 0)
			SERVER = true;
		else if (strlen(argv[i]) >= 4 && strcmp(argv[i] + strlen(argv[i]) - 4, ".lab") == 0) {
			cout << "Loading labyrinth from file " << argv[i] << " ";
			cout.flush();
			FILE *in = fopen(argv[i], "rb");
			if (read(map, in)) {
				cout << "[ok]" << endl;
				cout << "Checking labyrinth" << endl;
				check_labyrinth(map);
				cout << "Done!" << endl;
				loaded = true;
			} else {
				cout << "[fail]" << endl;
				map = Labyrinth();
			}
			fclose(in);
		} else {
			cout << "Unknown argument: " << argv[i] << endl;
			return 0;
		}
	}
	if (loaded)
		play(map);
	else if (!SERVER) {
		params = get_information();
		map = generate_labyrinth(params);
		play(map);
	} else {
		set_labyrinth_for_server(params);
		map = generate_labyrinth(params);
		server.init_server();
		server.clear_messages();
		thread t(accept_new_users, &server, &map, &params);
		play(map);
	}
}
