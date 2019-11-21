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
#include <sstream> 
#include <algorithm>
#include <time.h>
using namespace std;
struct  itemData
{
	int id;
	char icon;

	string name;
	int value;
	int Special_power;
	int Special_type;
	bool stackable;
};
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
vector <itemData> itemsData;
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
string const itemsData_path("data/items.txt");
fstream itemsData_f;
ofstream itemsData_of;

int size_x = 90, size_y = 90;
char Map[90][90];
int start_x, start_y;
char inputer;
char player_input;
char options_input;
char inv_input;
char inmovable[2] = { '#','~' };
HANDLE static hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//Options
bool PopUpPause;
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
	cout << "Coins:" << players[pl].coins;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

}
int randomize(int min, int max) {
	return rand() % (max - min + 1) + min;
}
void addMana(int count, int pl) {
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
	addHP(-1, pl);
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
		addMana(co, pl);
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
		addHP(co, pl);
		Map[players[pl].player_x][players[pl].player_y] = '_';
		waiter();
		break;
	}
	}
}
bool player_move_checker(int a, int b, int pl) {
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
						SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

					}
					else {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						//SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
					}


					cout << (char)206;
					//cout << "@";
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
				else if (Map[x][y] == 'S') {
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
					cout << "S";
				}
				else if (Map[x][y] == 'R') {
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					cout << "O";
				}
				else if (Map[x][y] == 'O') {
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

					cout << (char)153;

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
					if (Map[x][y] != 0) {
						cout << Map[x][y];
					}
				}
			}

		}
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		
		if (x == 0) {
			cout << "   ";
			drawStats(0);
		}
		else if (x == 1) {
			cout << "   ";
			drawStats(1);
		}
		else if (x == 2) {
			cout << "   ";
			cout << "'wasd' to move.'p' to check player's position.'x' to quit.";
				
		}
		else if (x == 3) {
			cout << "   ";
			cout << "'z' to save and load data.'o' to open options.";
				
		}
		else if (x == 4) {
			cout << "   ";
			cout << "'u' to open Player 0's inventory.'i' to open Player 1's inventory.";
		}
		else if (x == 5) {
			cout << "   ";
			cout << "'l' to open item's data.";
		}
		if (Map[x][0] != 0||x<7) {
			cout << endl;
		}
	}
}

void drawControls()
{
	cout << "'wasd' to move.'p' to check player's position.'x' to quit.\n'z' to save and load data.'o' to open options.\n'u' to open Player 0's inventory.'i' to open Player 1's inventory." << endl;
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
void addItem(item it, int pl) {
	if (itemsData[it.id].stackable) {
		for (int i = 0; i < players[pl].items.size(); i++)
		{
			if (players[pl].items[i].id == it.id) {
				players[pl].items[i].number += it.number;
				return;
			}
		}


	}
	cout << "adding unstackable. id:"<<it.id;
	players[pl].items.push_back(it);
	cout << "added id" <<players[pl].items[players[pl].items.size()-1].id;


}
void getItemsData() {
	itemsData_f.close();
	itemsData_f.open(itemsData_path.c_str(), ios::in | ios::out);

	int coun = 0;
	int onecoun = 0;
	vector<itemData> tet;
	itemData ne;
	string inp = "";

	while (getline(itemsData_f, inp))
	{

		switch (onecoun)
		{
		case 0: {
			stringstream geek(inp);


			geek >> ne.id;

			break;
		}
		case 1: {
			ne.icon = inp[0];
			break;
		}
		case 2: {
			ne.name = inp;

			break;
		}
		case 3: {
			stringstream geek(inp);


			geek >> ne.value;

			break;
		}
		case 4: {
			stringstream geek(inp);


			geek >> ne.Special_power;

			break;
		}
		case 5: {
			stringstream geek(inp);


			geek >> ne.Special_type;

			break;
		}
		case 6: {
			stringstream geek(inp);

			int nn;
			geek >> nn;
			ne.stackable = nn;
			tet.push_back(ne);
			onecoun = -1;
			break;
		}
		default:
			break;
		}
		onecoun++;
	}

	itemsData = tet;
}
void drawItemsData() {
	system("cls");
	getItemsData();

	cout << "Items's Data" << endl;
	for (int i = 0; i < itemsData.size(); i++)
	{
		cout << itemsData[i].icon;
		cout << " ID:" << itemsData[i].id;
		cout << " Name:" << itemsData[i].name;
		cout << " value:" << itemsData[i].value;
		cout << " Special Power:" << itemsData[i].Special_power;
		cout << " Special type:" << itemsData[i].Special_type;
		cout << " Stackable?:" << itemsData[i].stackable << endl;




	}
	_getch();
}
void getInventory() {
	{
		inv1_f.close();
		inv1_f.open(inv1_path.c_str(), ios::in | ios::out);

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
		inv2_f.close();
		inv2_f.open(inv2_path.c_str(), ios::in | ios::out);
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
}
void setInventory() {

	int siz = players[0].items.size();
	cout << "siz0" << siz;

	inv1_of.close();
	inv1_of.open(inv1_path.c_str(), ios::in | ios::out);
	for (int i = 0; i < siz; i++)
	{
		inv1_of << players[0].items[i].id << '\n';
		inv1_of << players[0].items[i].number << '\n';
		cout << "id" << players[0].items[i].id << "num" << players[0].items[i].number;

	}


	siz = players[1].items.size();
	cout << "siz1" << siz;

	inv2_of.close();
	inv2_of.open(inv2_path.c_str(), ios::in | ios::out);
	for (int i = 0; i < siz; i++)
	{
		inv2_of << players[1].items[i].id << '\n';
		inv2_of << players[1].items[i].number << '\n';
		cout << "id" << players[1].items[i].id << "num" << players[1].items[i].number;

	}


}
void setPlayerStats() {
	stats_of.close();
	stats_of.open(stats_path.c_str(), ios::in | ios::out);
	for (int i = 0; i < 2; i++)
	{
		stats_of << players[0].hp << '\n';
		stats_of << players[0].max_hp << '\n';
		stats_of << players[0].mana << '\n';
		stats_of << players[0].max_mana << '\n';
		stats_of << players[0].coins << '\n';
	}


}
void getPlayerStats() {
	int data[20];
	int value = 0;
	int nl = 0;
	while (stats_f >> value) {
		data[nl] = value;
		nl++;
	}

	for (int plc = 0; plc < 2; plc++)
	{
		players[plc].hp = data[plc*data_size + 0];
		players[plc].max_hp = data[plc * data_size + 1];
		players[plc].mana = data[plc * data_size + 2];
		players[plc].max_mana = data[plc *data_size + 3];
		players[plc].coins = data[plc * data_size + 4];
	}
}
void setSettings() {
	settings_of.close();
	settings_of.open(settings_path.c_str(), ios::in | ios::out);
	settings_of << PopUpPause;
	settings_of.flush();
}
void getSettings() {
	settings_f.close();
	settings_f.open(settings_path.c_str(), ios::in | ios::out);
	bool settinhstab[10];
	bool inputter = 0;
	int conter = 0;
	while (settings_f >> inputter) {
		settinhstab[conter] = inputter;
		conter++;
	}
	PopUpPause = settinhstab[0];
}
void getMap() {
	map_f.close();
	map_f.open(map_path.c_str(), ios::in | ios::out);
	int a = 0;
	int b = 0;

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
}
void drawInventory(int pl) {
	system("cls");


	cout << "Player " << pl << "'s inventory" << endl;
	int n = players[pl].items.size();
	for (int i = 0; i < n; i++)
	{
		itemData iD = itemsData[players[pl].items[i].id];
		if (iD.stackable) {
			cout << players[pl].items[i].number << "x ";
		}
		cout << iD.icon << " " << iD.name << ". Value: " << iD.value;



		cout << endl;
	}
	cout << "'u' to open Player 0's inventory.'i' to open Player 1's inventory.'r' to add rock to Player " << pl << ".'x' to exit this menu" << endl;


	inv_input = _getch();


	switch (inv_input) {
	case 'u': {
		drawInventory(0);

		break;
	}
	case 'i': {
		drawInventory(1);

		break;
	}
	case 'r': {
		item it;
		it.id = 0;
		it.number = 1;
		addItem(it, pl);
		drawInventory(pl);


		break;
	}
	case 't': {
		item it;
		it.id = 1;
		it.number = 1;
		addItem(it, pl);
		drawInventory(pl);


		break;
	}
	case 'x': {
		return;
	}
	default:
		return;
	}
}
int main()
{
	srand(time(NULL));


	getItemsData();
	getInventory();

	getSettings();
	getMap();

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
		drawMap(0);
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


			setSettings();
			setPlayerStats();
			setInventory();
			getMap();
			return 0;

			break;
		}
		case 'z':
		{


			getMap();
			setSettings();
			getSettings();
			setPlayerStats();
			getPlayerStats();
			setInventory();
			getInventory();
			break;
		}
		case 'o':
		{
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
		case 'u': {
			drawInventory(0);

			break;
		}
		case 'i': {
			drawInventory(1);

			break;
		}

		case 'l': {
			drawItemsData();

			break;
		}


		}
		}
		check_player_position(0);
		check_player_position(1);


	}

}