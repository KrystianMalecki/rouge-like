/* you can use these constants
FOREGROUND_BLUE
FOREGROUND_GREEN
FOREGROUND_RED
FOREGROUND_INTENSITY
BACKGROUND_BLUE
BACKGROUND_GREEN
BACKGROUND_RED
BACKGROUND_INTENSITY
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << (char)207;
*/
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <conio.h>
#include <iomanip>
#include <ctime>
#include <vector>
#include <time.h>
using namespace std;

//Player's Data
int player_x, player_y;
int hp = 10, max_hp = 10;
int mana = 10, max_mana = 10;
int coins;

//Static Data
string const map_path("map/map.txt");
ofstream map_of;
fstream map_f;
string const inv_path("player/items.txt");
ofstream inv_of;
fstream inv_f;
string const stats_path("player/stats.txt");
fstream stats_f;
ofstream stats_of;
string const settings_path("player/settings.txt");
fstream settings_f;
ofstream settings_of;
int size_x = 20, size_y = 20;
char Map[20][20];
int start_x, start_y;
char inputer;
char player_input;
char options_input;
char inmovable[2] = { '#','~' };
HANDLE static hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//Options
bool PopUpPause;

int randomize(int min, int max) {
	return rand() % (max - min + 1) + min;
}
void addMana(int count) {
	mana += count;
	if (mana > max_mana) {
		mana = max_mana;
	}
}
void addHP(int count) {
	hp += count;
	if (hp > max_hp) {
		hp= max_hp;
	}
}
void waiter() {
	if (PopUpPause) {
		system("pause");

	}
}
void check_player_position() 
{

	char buff = Map[player_x][player_y];
	switch (buff) {
	case 'T':
	{	cout << "You stepped on a trap! -1 HP!";
	hp--;
	Map[player_x][player_y] = 't';
	waiter();
	break;
	}
	case 'o':
	{	
		int co = randomize(1, 3);
		if (co == 1) {
			cout << "You collected " << co << " coin!";
		}
		else {
			cout << "You collected " << co << " coins!";

		}

	coins+=co;
	Map[player_x][player_y] = '_';
	waiter();	break;
	}
	case 'm':
	{	
		int co = randomize(1, 3);
		if (co == 1) {
			cout << "You gained " << co << " mana point!";
		}
		else {
			cout << "You gained " << co << " mana points!";

		}
		addMana(co);
	Map[player_x][player_y] = '_';
	waiter();	break;
	}
	case '+':
	{
		int co = randomize(1, 3);
		if (co == 1) {
			cout << "You gained " << co << " hp!";
		}
		else {
			cout << "You gained " << co << " hp!";

		}
		addHP(co);
		Map[player_x][player_y] = '_';
		waiter();
		break;
	}
	}
}
	bool player_move_checker(int a, int b) {
		int counter = 0;
		int sizee = sizeof(inmovable) / sizeof(inmovable[0]);
		for (int i = 0; i < sizee; i++)
		{
			if (Map[player_x + a][player_y + b] == inmovable[i]) {
				counter++;
			}
		}
		return (counter == 0);
	}
void drawMap()
	{
		system("cls");


		for (int x = 0; x < size_x; x++)
		{
			for (int y = 0; y < size_y; y++)
			{

				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				if (x == player_x && y == player_y)
				{
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
					cout << "@";
				}
				else {
					if (Map[x][y] == 'm') {
						SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						cout << "+";
					}
					else if (Map[x][y] == '+') {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
						cout << "+";
					}
					else if (Map[x][y] == '~') {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
						int n = randomize(0, 4);
						switch (n) {
						case 0:
						{
							cout << "~";
							break; }
						case 1:
						{
							cout << "-";
							break; }
						case 2:
						{
							cout << (char)246;
							break;
						}

						case 3:
						{
							cout << (char)248;
							break;
						}
						case 4:
						{
							cout << (char)250;
							break;
						}
						}
					}
					else if (Map[x][y] == '#') {
						SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY);
						cout << Map[x][y];
					}
					else if (Map[x][y] == 'T') {
						//SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_GREEN);
						//cout << (char)207;
						cout << "_";
					}
					else if (Map[x][y] == 't') {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
						cout << (char)207;

					}
					else if (Map[x][y] == 'o') {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN|FOREGROUND_INTENSITY);
						cout << "o";

					}
					else
					{
						cout << Map[x][y];
					}
				}
			}
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			cout << endl;
		}
	}
	void drawStats() 
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "HP:" << hp << "/" << max_hp << " <3";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << " | ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "Mana:" << mana << "/" << max_mana ;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << " | ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "Coins:" <<coins<< endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

	}
	void drawControls() 
	{
		cout << "'wasd' to move.'p' to check player's position.'x' to quit.\n'z' to save and load data.'o' to open options." << endl;
	}
void drawOptions() {

		if (PopUpPause) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "o";
		}
		else {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "x";
		}
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

		cout << "- Pause game after pop-up. 'p' to toggle." << endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

		cout << "'o' to exit." << endl;
	}

int main()
{
	srand(time(NULL));

	map_f.open(map_path.c_str(), ios::in | ios::out);
	map_of.open(map_path.c_str(), ios::in | ios::out);
	inv_f.open(inv_path.c_str(), ios::in | ios::out);
	inv_of.open(inv_path.c_str(), ios::in | ios::out);
	stats_f.open(stats_path.c_str(), ios::in | ios::out);
	stats_of.open(stats_path.c_str(), ios::in | ios::out);
	settings_f.open(settings_path.c_str(), ios::in | ios::out);
	settings_of.open(settings_path.c_str(), ios::in | ios::out);
	
	bool sett1[10];
	bool sett2 = 0;
	int sett3 = 0;
	while (settings_f >> sett2) {
		sett1[sett3] = sett2;
		sett3++;
	}
	PopUpPause = sett1[0];
	int a = 0;
	int b = 0;
	int n[10];
	int value = 0;
	int nl = 0;
	while (stats_f >> value) {
		n[nl] = value;
		nl++;
	}
	hp = n[0];
	max_hp = n[1];
	mana = n[2];
	max_mana = n[3];
	coins = n[4];
	while (map_f >> inputer) {

		if (inputer == 'l')
		{
			a++;
			b = 0;
			continue;
		}
		Map[a][b] = inputer;
		b++;

	}

	for (int x = 0; x < size_x; x++)
	{
		for (int y = 0; y < size_y; y++)
		{
			if (Map[x][y] == '%')
			{
				Map[x][y] = '_';
				start_x = x;
				start_y = y;
				player_x = start_x;
				player_y = start_y;

			}
		}
	}


	//cout << nn;
	while (true) {
		drawMap();

		drawStats();
		drawControls();



		player_input = _getch();


		switch (player_input) {
		case 'w':
		{
			if (player_move_checker(-1, 0)) {
				player_x--;
			}
			break;
		}
		case 'a':
		{
			if (player_move_checker(0, -1)) {
				player_y--;
			}
			break;
		}
		case 's':
		{
			if (player_move_checker(1, 0)) {
				player_x++;
			}
			break;
		}
		case 'd':
		{
			if (player_move_checker(0, 1)) {
				player_y++;
			}
			break;
		}
		case 'p':
		{
			cout << "x:" << player_x << "|y:" << player_y << endl;
			waiter();
			break;
		}
		case 'x':
		{
			stats_of.close();
			stats_of.open(stats_path.c_str(), ios::in | ios::out);

			stats_of << hp << '\n';
			stats_of << max_hp << '\n';
			stats_of << mana << '\n';
			stats_of << max_mana << '\n';
			stats_of << coins << '\n';
			settings_f.close();
			settings_f.open(settings_path.c_str(), ios::in | ios::out);
			settings_of << PopUpPause;
			settings_of.flush();
			return 0;

			break;
		}
		case 'z':
		{
			stats_of.close();
			stats_of.open(stats_path.c_str(), ios::in | ios::out);
			stats_f.close();
			stats_f.open(stats_path.c_str(), ios::in | ios::out);
			settings_f.close();
			settings_f.open(settings_path.c_str(), ios::in | ios::out);
			stats_of << hp << '\n';
			stats_of << max_hp << '\n';
			stats_of << mana << '\n';
			stats_of << max_mana << '\n';
			stats_of << coins << '\n';
			int a = 0;
			int b = 0;
			int n[10];
			int value = 0;
			int nl = 0;
			stats_f.flush();
			while (stats_f >> value) {
				n[nl] = value;
				nl++;
			}
			hp = n[0];
			max_hp = n[1];
			mana = n[2];
			max_mana = n[3];
			coins = n[4];
			map_f.flush();

			while (map_f >> inputer) {

				if (inputer == 'l')
				{
					a++;
					b = 0;
					continue;
				}
				Map[a][b] = inputer;
				b++;

			}
			settings_of << PopUpPause;
			settings_of.flush();
			bool sett1[10];
			bool sett2 = 0;
			int sett3 = 0;
			while (settings_f >> sett2) {
				sett1[sett3] = sett2;
				sett3++;
			}
			PopUpPause = sett1[0];
			break;
		}
		case 'o':
		{
			bool opt = true;
			while (opt) {
				system("cls");
				cout << "Options" << endl;
				drawOptions();
				options_input = _getch();


				switch (options_input) {
				case 'o':
				{
					opt = false;
					break;
				}
				case 'p':
				{
					PopUpPause = !PopUpPause;
					break;
				}
				default:
				{
					opt = false;
					break;
				}

				}
			}
			break; }
		default: {break; }
		}
		check_player_position();
		}
	}
