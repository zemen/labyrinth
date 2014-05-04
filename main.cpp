/* Labyrinth Game v10
 *
 * by pershik & matushkin & zemen
 *
 * Changelog:
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
 * - Added a second exit in labyrinth
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
#include "gen.cpp"
#include "init.cpp"
#include "turn.cpp"
#include "game.cpp"
#include "debug.cpp"
//~ #include "save.cpp"

int main(int argc, char **argv) {
	for (int i = 0; i < argc; ++i)
		if (strcmp(argv[i], "DEBUG") == 0)
			DEBUG = true;
	init();
	Parameters params = get_information();
	Labyrinth map = generate_labyrinth(params);
	play(map);
}
