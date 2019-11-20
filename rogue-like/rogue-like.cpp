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

#include <algorithm>
#include <time.h>
using namespace std;
//Structs
struct  item
{
	int id;
	int number;
};
//Player's Data
struct  player
{
	int player_x;
	int player_y;
	int hp = 10;
	int max_hp = 10;
	int mana = 10;
	int max_mana = 10;
	int coins;
	vector <item> items;
};
int data_size = 5;
player players[2];
//int curr_pl;
//Static Data
string const map_path("map/map.txt");
ofstream map_of;
fstream map_f;
string const inv1_path("player/items1.txt");
ofstream inv1_of;
fstream inv1_f;
string const inv2_path("player/items2.txt");
ofstream inv2_of;
fstream inv2_f;
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
void addMana(int count,int pl) {
	players[pl].mana += count;
	if (players[pl].mana > players[pl].max_mana) {
		players[pl].mana = players[pl].max_mana;
	}
}
void addHP(int count, int pl) {
	players[pl].hp += count;
	if (players[pl].hp > players[pl].max_hp) {
		players[pl].hp = players[pl].max_hp;
	}
}
void waiter() {
	if (PopUpPause) {
		system("pause");

	}
}

void check_player_position(int pl)
{
	
	
	
	
	char buff = Map[players[pl].player_x][players[pl].player_y];
	switch (buff) {
	case 'T':
	{	cout << "You stepped on a trap! -1 HP!";
	addHP(-1,pl);
	Map[players[pl].player_x][players[pl].player_y] = 't';
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

		players[pl].coins += co;
		Map[players[pl].player_x][players[pl].player_y] = '_';
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
		addMana(co,pl);
		Map[players[pl].player_x][players[pl].player_y] = '_';
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
		addHP(co,pl);
		Map[players[pl].player_x][players[pl].player_y] = '_';
		waiter();
		break;
	}
	}
}
bool player_move_checker(int a, int b,int pl) {
	int counter = 0;
	int sizee = sizeof(inmovable) / sizeof(inmovable[0]);
	for (int i = 0; i < sizee; i++)
	{
		if (Map[players[pl].player_x + a][players[pl].player_y + b] == inmovable[i]) {
			counter++;
		}
	}
	return (counter == 0);
}
void drawMap(int pl)
{
	system("cls");

	
	for (int x = 0; x < size_x; x++)
	{
		for (int y = 0; y < size_y; y++)
		{
			bool rend_pl = false;
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			for (int plc = 0; plc < 2; plc++)
			{
				if (x == players[plc].player_x && y == players[plc].player_y)
				{
					if (plc == 0) {

						SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
						
					}
					else {

						SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
					}
					cout << "@";
					rend_pl = true;
					plc = 2;
				}
			}
			if (!rend_pl) {
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
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
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

void drawStats(int pl)
{
	
	if (pl == 0) {

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

	}
	else {

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	}
	cout << "Player " << pl << " : ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "HP:" << players[pl].hp << "/" << players[pl].max_hp << " <3";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << " | ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Mana:" << players[pl].mana << "/" << players[pl].max_mana;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << " | ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Coins:" << players[pl].coins << endl;
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
	
	stats_f.open(stats_path.c_str(), ios::in | ios::out);
	stats_of.open(stats_path.c_str(), ios::in | ios::out);
	settings_f.open(settings_path.c_str(), ios::in | ios::out);
	settings_of.open(settings_path.c_str(), ios::in | ios::out);
	{
		int it=0;
		bool ided = false;
		vector<item> tet;
		item ne;
		while (inv1_f >> it) {
			if (ided) {
				ne.number = it;
				tet.push_back(ne);
				ided = false;
			}
			else {
				ne.id = it;
				ided = true;

			}
		}
		players[0].items = tet;
		 it=0;
		 ided = false;
		
		 tet.clear();
		while (inv2_f >> it) {
			if (ided) {
				ne.number = it;
				tet.push_back(ne);
				ided = false;
			}
			else {
				ne.id = it;
				ided = true;

			}
		}
		players[1].items = tet;
	}
	{
		
		int siz = players[0].items.size();
		inv1_of.close();
		inv1_of.open(stats_path.c_str(), ios::in | ios::out);
		for (int i = 0; i < siz; i++)
		{
			inv1_of << players[0].items[i].id<<'\n';
			inv1_of << players[0].items[i].number << '\n';

		}
		

		 siz = players[1].items.size();
		inv2_of.close();
		inv2_of.open(stats_path.c_str(), ios::in | ios::out);
		for (int i = 0; i < siz; i++)
		{
			inv2_of << players[1].items[i].id << '\n';
			inv2_of << players[1].items[i].number << '\n';

		}
		
		
	}
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
	int n[20];
	int value = 0;
	int nl = 0;
	while (stats_f >> value) {
		n[nl] = value;
		nl++;
	}
	
	for (int plc = 0; plc < 2; plc++)
	{
		players[plc].hp = n[plc*data_size +0];
		players[plc].max_hp = n[plc * data_size + 1];
		players[plc].mana = n[plc * data_size + 2];
		players[plc].max_mana = n[plc *data_size + 3];
		players[plc].coins = n[plc * data_size + 4];
	}
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
				players[0].player_x = start_x;
				players[0].player_y = start_y;
				players[1].player_x = start_x;
				players[1].player_y = start_y;
			}
			
		}
	}
	while (true) {
	
	//	cout << curr_pl;

		//cout << nn;
		
			drawMap(0);

			drawStats(0);
			drawStats(1);

			drawControls();



			player_input = _getch();


			switch (player_input) {
			case 'w':
			{
				if (player_move_checker(-1, 0, 0)) {
					players[0].player_x--;
				}
				break;
			}
			case 'a':
			{
				if (player_move_checker(0, -1, 0)) {
					players[0].player_y--;
				}
				break;
			}
			case 's':
			{
				if (player_move_checker(1, 0, 0)) {
					players[0].player_x++;
				}
				break;
			}
			case 'd':
			{
				if (player_move_checker(0, 1, 0)) {
					players[0].player_y++;
				}

				break;

			}
			case 'p':
			{
				cout << "Player " << 0 << ": x:" << players[0].player_x << "|y:" << players[0].player_y << endl;
				cout << "Player " << 1 << ": x:" << players[1].player_x << "|y:" << players[1].player_y << endl;

				waiter();
				break;
			}
			case 'x':
			{
				stats_of.close();
				stats_of.open(stats_path.c_str(), ios::in | ios::out);

				stats_of << players[0].hp << '\n';
				stats_of << players[0].max_hp << '\n';
				stats_of << players[0].mana << '\n';
				stats_of << players[0].max_mana << '\n';
				stats_of << players[0].coins << '\n';
				stats_of << players[1].hp << '\n';
				stats_of << players[1].max_hp << '\n';
				stats_of << players[1].mana << '\n';
				stats_of << players[1].max_mana << '\n';
				stats_of << players[1].coins << '\n';
				settings_f.close();
				settings_f.open(settings_path.c_str(), ios::in | ios::out);
				settings_of << PopUpPause;
				settings_of.flush();
				{

					int siz = players[0].items.size();
					inv1_of.close();
					inv1_of.open(stats_path.c_str(), ios::in | ios::out);
					for (int i = 0; i < siz; i++)
					{
						inv1_of << players[0].items[i].id << '\n';
						inv1_of << players[0].items[i].number << '\n';

					}


					 siz = players[1].items.size();
					inv2_of.close();
					inv2_of.open(stats_path.c_str(), ios::in | ios::out);
					for (int i = 0; i < siz; i++)
					{
						inv2_of << players[1].items[i].id << '\n';
						inv2_of << players[1].items[i].number << '\n';

					}


				}
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
				stats_of << players[0].hp << '\n';
				stats_of << players[0].max_hp << '\n';
				stats_of << players[0].mana << '\n';
				stats_of << players[0].max_mana << '\n';
				stats_of << players[0].coins << '\n';
				stats_of << players[1].hp << '\n';
				stats_of << players[1].max_hp << '\n';
				stats_of << players[1].mana << '\n';
				stats_of << players[1].max_mana << '\n';
				stats_of << players[1].coins << '\n';
				int a = 0;
				int b = 0;
				int n[20];
				int value = 0;
				int nl = 0;
				while (stats_f >> value) {
					n[nl] = value;
					nl++;
				}

				for (int plc = 0; plc < 2; plc++)
				{
					players[plc].hp = n[plc*data_size + 0];
					players[plc].max_hp = n[plc * data_size + 1];
					players[plc].mana = n[plc * data_size + 2];
					players[plc].max_mana = n[plc *data_size + 3];
					players[plc].coins = n[plc * data_size + 4];
				}
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
				{

					int siz = players[0].items.size();
					inv1_of.close();
					inv1_of.open(stats_path.c_str(), ios::in | ios::out);
					for (int i = 0; i < siz; i++)
					{
						inv1_of << players[0].items[i].id << '\n';
						inv1_of << players[0].items[i].number << '\n';

					}


					 siz = players[1].items.size();
					inv2_of.close();
					inv2_of.open(stats_path.c_str(), ios::in | ios::out);
					for (int i = 0; i < siz; i++)
					{
						inv2_of << players[1].items[i].id << '\n';
						inv2_of << players[1].items[i].number << '\n';

					}


				}
				{
					int it = 0;
					bool ided = false;
					vector<item> tet;
					item ne;
					while (inv1_f >> it) {
						if (ided) {
							ne.number = it;
							tet.push_back(ne);
							ided = false;
						}
						else {
							ne.id = it;
							ided = true;

						}
					}
					players[0].items = tet;
					it = 0;
					ided = false;

					tet.clear();
					while (inv2_f >> it) {
						if (ided) {
							ne.number = it;
							tet.push_back(ne);
							ided = false;
						}
						else {
							ne.id = it;
							ided = true;

						}
					}
					players[1].items = tet;
				}
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
			case 72:
			{
				if (player_move_checker(-1, 0, 1)) {
					players[1].player_x--;
				}
				break;
			}
			case 80:
			{
				if (player_move_checker(1, 0, 1)) {
					players[1].player_x++;
				}
				break;
			}
			case 77:
			{
				if (player_move_checker(0, 1, 1)) {
					players[1].player_y++;
				}

				break;

			}			case 75:
			{
				if (player_move_checker(0, -1, 1)) {
					players[1].player_y--;
				}
				break;
			}
			default: {break; }
			}
			check_player_position(0);
			check_player_position(1);

		}
	}

