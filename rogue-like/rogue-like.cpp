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
	char icon;

	string name;
	int value;
	int Special_power;
	int Special_type;
	bool stackable;
	int Special_value;
};
//Structs
struct  item
{
	string name;
	int number;
};
struct  player
{
	bool alive = true;
	int player_x;
	int player_y;
	int hp = 10;
	int max_hp = 10;
	int mana = 10;
	int max_mana = 10;
	int coins;
	int attack;
	int lvl = 0;
	int xp = 0;
	vector <item> items;
};
struct enemy {
	string name;
	char icon;
	int max_hp = 10;
	int dmg = 1;
	float hp_scal = 0.1f;
	float dmg_scal = 0.1f;

};
//Enemies
vector <enemy> Enemies;
//Player's Data
vector <itemData> itemsData;
int data_size = 9;
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
string const settings_path("data/settings.txt");
fstream settings_f;
ofstream settings_of;
string const itemsData_path("data/items.txt");
fstream itemsData_f;
ofstream itemsData_of;
string const enemiesData_path("data/enemies.txt");
fstream enemiesData_f;
ofstream enemiesData_of;
int size_x = 20, size_y = 20;
char Map[20][20];
int start_x, start_y;
char inputer;
char player_input;
char options_input;
char combat_input;
char inv_input;
char inmovable[2] = { '#','~' };
HANDLE static hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//Options
bool PopUpPause;
bool DataOnRight;
bool stupid;
//
int check(string name) {
	for (int i = 0; i < itemsData.size(); i++)
	{
		if (itemsData[i].name == name) {
			return i;
		}
	}
}
void drawChars() {
	system("cls");
	for (int i = 0; i < 257; i++)
	{
		cout << i << " - " << (char)i << endl;
	}
	system("pause");
}
void drawStats(int pl)
{

	if (pl == 0) {

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

	}
	else {

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	}
	if (!stupid) {
		cout << "Player " << pl << " : ";
	}
	else {
		cout << "Player " << pl + 1 << " : ";

	}
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
	cout << " | ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "LVL:" << players[pl].lvl << " ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "Xp:" << players[pl].xp << "/" << (players[pl].lvl*(10 + players[pl].lvl)) << " ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	int xpsum = (int)(((players[pl].xp * 1.0) / (players[pl].lvl*(10 + players[pl].lvl)))*10.0);
	for (int i = 0; i < xpsum; i++)
	{
		cout << (char)219;
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);

	for (int i = 0; i < 10 - xpsum; i++)
	{
		cout << (char)219;
	}
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
void addXP(int count, int pl) {
	players[pl].xp += count;
	int nextlvl = players[pl].lvl*(10 + players[pl].lvl);
	if (players[pl].xp >= nextlvl) {

		players[pl].lvl++;

		cout << "You advanced to level " << players[pl].lvl << "!" << endl;

		players[pl].xp -= nextlvl;
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
void Combat(int pl, string type, int lvl) {
	int k;
	for (k = 0; k < Enemies.size(); k++)
	{
		if (Enemies[k].name == type) {
			break;
		}
	}
	if (Enemies.size() == k) {
		return;
	}
	enemy currEnemy = Enemies[k];
	currEnemy.dmg += currEnemy.dmg_scal*lvl;
	currEnemy.max_hp += currEnemy.hp_scal*lvl;

	int enemy_hp = currEnemy.max_hp;

	while (true) {
		system("cls");

		cout << "Fight with " << Enemies[k].name << endl;
		drawStats(pl);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << " | ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Attack:" << players[pl].attack << endl;

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

		if (type == "Orc") {
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

		}
		cout << type << " : ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << " HP:" << enemy_hp << "/" << currEnemy.max_hp << " <3";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << " | ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Attack:" << currEnemy.dmg;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << " | ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "LVL:" << lvl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << endl;
		cout << "1. Attack for " << players[pl].attack << endl;
		cout << "2. Heal for " << 2 << ". Cost: " << 2;
		if (2 == 1) {
			cout << " mana point" << endl;
		}
		else {
			cout << " mana points" << endl;

		}
		cout << "3. Run" << endl;
		combat_input = _getch();

		switch (combat_input) {
		case '1': {
			enemy_hp -= 5;// players[pl].attack;
			break;
		}
		case '2': {
			addHP(2, pl);
			addMana(-2, pl);

			break;
		}
		case '3': {
			addHP(-currEnemy.dmg, pl);
			Map[players[pl].player_x][players[pl].player_y] = '_';

			return;
			break;
		}
		default:
			break;
		}
		{
			addHP(-currEnemy.dmg, pl);

		}
		if (players[pl].hp <= 0) {
			Map[players[pl].player_x][players[pl].player_y] = '_';

			return;
		}
		if (enemy_hp <= 0) {

			cout << endl;
			cout << "You killed " << currEnemy.name << "!";
			cout << endl;

			if (lvl * 5 == 1) {
				cout << "You collected " << lvl * 5 << " coin!";
			}
			else {
				cout << "You collected " << lvl * 5 << " coins!";

			}
			cout << endl;

			cout << "You gained " << lvl * 5 << " xp!";
			cout << endl;

			players[pl].coins += lvl * 5;
			addXP(lvl * 5, pl);
			Map[players[pl].player_x][players[pl].player_y] = '_';

			waiter();
			return;
		}
	}
}
void check_player_position(int pl)
{
	if (players[pl].hp <= 0) {
		players[pl].alive = false;
		players[pl].player_x = -1;
		players[pl].player_y = -1;
		if (!stupid) {
			cout << "Player " << pl << " died!" << endl;
		}
		else {
			cout << "Player " << pl + 1 << " died!" << endl;

		}
		int mm = randomize(0, 100);
		if (mm == 69) {
			cout << "Take the L, Loser!" << endl;
			cout << "pulls both arms outwards in front of my chest and pumps them behind my back, repeats this motion in a smaller range of motion down to my hips two times once more all while sliding my legs in a faux walking motion, claps my hands together in front of me while both my knees knock together, pumps my arms downward, pronating my wrists and abducting my fingers outward while crossing my legs back and forth, repeats this motion again two times while keeping my shoulders low and hunching over, does finger gun with right hand with left hand bent on my hip while looking directly forward and putting my left leg forward then crossing my arms and leaning back a little while bending my knees at an angle" << endl;
		}
		waiter();

	}
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
	case 'R':
	{
		int lvl = randomize(1, 5);
		Combat(pl, "Orc", lvl);
		break;
	}

	}
}
bool player_move_checker(int a, int b, int pl) {
	if (!players[pl].alive) {
		return false;
	}
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
void drawControls(int i)
{
	if (i == 0) {
		drawStats(0);
	}
	else if (i == 1) {
		drawStats(1);
	}
	else if (i == 2) {
		cout << "'wasd' to move.'p' to check player's position.'x' to quit.";

	}
	else if (i == 3) {
		cout << "'z' to save and load data.'o' to open options.";

	}
	else if (i == 4) {
		if (!stupid) {
			cout << "'u' to open Player 0's inventory.'i' to open Player 1's inventory.";
		}
		else {
			cout << "'u' to open Player 1's inventory.'i' to open Player 2's inventory.";

		}
	}
	else if (i == 5) {
		cout << "'l' to open item's data.'c' to draw chars.";
	}

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


					cout << (char)203;
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
					cout << (char)177;
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
				else if (Map[x][y] == 'd') {
					SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN);
					cout << (char)170;
				}
				else if (Map[x][y] == '0') {
					cout << ' ';
				}
				else
				{

					cout << Map[x][y];

				}
			}

		}
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		if (DataOnRight) {
			cout << "   ";
			drawControls(x);
		}

		cout << endl;

	}
}
void show_o_x(bool b) {
	if (b) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "o";
	}
	else {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "x";
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

}
void drawOptions() {


	show_o_x(PopUpPause);
	cout << "- Pause game after pop-up. 'q' to toggle." << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	show_o_x(DataOnRight);
	cout << "- Draw Player's Data and controls on right part of the screen. 'w' to toggle." << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	show_o_x(stupid);
	cout << "- I'm fucking stupid. 'e' to toggle." << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

	cout << "'o' to exit." << endl;
}
void addItem(item it, int pl) {
	int idd = check(it.name);
	if (itemsData[idd].stackable) {
		for (int i = 0; i < players[pl].items.size(); i++)
		{
			if (players[pl].items[i].name == it.name) {
				players[pl].items[i].number += it.number;
				return;
			}
		}


	}
	cout << "adding unstackable. name:" << it.name;
	players[pl].items.push_back(it);
	cout << "added name" << players[pl].items[players[pl].items.size() - 1].name;


}
void getEnemiesData() {
	enemiesData_f.close();
	enemiesData_f.open(enemiesData_path.c_str(), ios::in | ios::out);

	int coun = 0;
	int onecoun = 0;
	vector<enemy> tet;
	enemy ne;
	string inp = "";

	while (getline(enemiesData_f, inp))
	{

		switch (onecoun)
		{
		case 0: {
			ne.name = inp;

			break;
		}
		case 1: {



			ne.icon = inp[0];

			break;
		}
		case 2: {
			stringstream geek(inp);


			geek >> ne.max_hp;


			break;
		}
		case 3: {
			stringstream geek(inp);


			geek >> ne.dmg;

			break;
		}
		case 4: {
			stringstream geek(inp);


			geek >> ne.hp_scal;

			break;
		}
		case 5: {
			stringstream geek(inp);


			geek >> ne.dmg_scal;
			break;
		}
		case 6: {


			onecoun = -1;
			tet.push_back(ne);
			break;
		}
		default:
			break;
		}
		onecoun++;
	}

	Enemies = tet;
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
			ne.name = inp;

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

			break;
		}
		case 7: {
			stringstream geek(inp);


			geek >> ne.Special_value;

			break;
		}
		case 8: {

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
		cout << " Name:" << itemsData[i].name;
		cout << " value:" << itemsData[i].value;
		cout << " Special Power:" << itemsData[i].Special_power;
		cout << " Special type:" << itemsData[i].Special_type;
		cout << " Stackable?:" << itemsData[i].stackable << endl;




	}
	_getch();
}
void getInventory() {

	inv1_f.close();
	inv1_f.open(inv1_path.c_str(), ios::in | ios::out);

	string inp = "";
	int cont = 0;
	vector<item> tet;
	item ne;
	while (getline(inv1_f, inp))
	{
		switch (cont)
		{
		case 0: {
			ne.name = inp;

			break;
		}
		case 1: {
			stringstream geek(inp);
			geek >> ne.number;
			break;
		}
		case 2: {
			cont = -1;
			tet.push_back(ne);
			break;
		}

		default:
			break;
		}
		cont++;
	}
	players[0].items = tet;
	inv2_f.close();
	inv2_f.open(inv2_path.c_str(), ios::in | ios::out);
	tet.clear();
	inp = "";
	cont = 0;


	while (getline(inv2_f, inp))
	{

		switch (cont)
		{
		case 0: {
			ne.name = inp;

			break;
		}
		case 1: {
			stringstream geek(inp);
			geek >> ne.number;
			break;
		}
		case 2: {
			cont = -1;
			tet.push_back(ne);
			break;
		}

		default:
			break;
		}
		cont++;
	}
	players[1].items = tet;



}
void setInventory() {

	int siz = players[0].items.size();
	cout << "Player 0" << endl;

	inv1_of.close();
	inv1_of.open(inv1_path.c_str(), ios::in | ios::out);
	for (int i = 0; i < siz; i++)
	{
		inv1_of << players[0].items[i].name << '\n';
		inv1_of << players[0].items[i].number << '\n';
		inv1_of << "------------------" << '\n';

		cout << "name" << players[0].items[i].name << "num" << players[0].items[i].number << endl;

	}


	siz = players[1].items.size();
	cout << "Player 1" << endl;


	inv2_of.close();
	inv2_of.open(inv2_path.c_str(), ios::in | ios::out);
	for (int i = 0; i < siz; i++)
	{
		inv2_of << players[1].items[i].name << '\n';
		inv2_of << players[1].items[i].number << '\n';
		cout << "name" << players[1].items[i].name << "num" << players[1].items[i].number << endl;

	}

}
void setPlayerStats() {
	stats_of.close();
	stats_of.open(stats_path.c_str(), ios::in | ios::out);
	for (int i = 0; i < 2; i++)
	{
		stats_of << players[i].hp << '\n';
		stats_of << players[i].max_hp << '\n';
		stats_of << players[i].mana << '\n';
		stats_of << players[i].max_mana << '\n';
		stats_of << players[i].coins << '\n';
		stats_of << players[i].player_x << '\n';
		stats_of << players[i].player_y << '\n';
		stats_of << players[i].lvl << '\n';
		stats_of << players[i].xp << '\n';

	}


}
void getPlayerStats() {
	stats_f.close();
	stats_f.open(stats_path.c_str(), ios::in | ios::out);
	int data[35];
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
		players[plc].player_x = data[plc * data_size + 5];

		players[plc].player_y = data[plc * data_size + 6];
		players[plc].lvl = data[plc * data_size + 7];
		players[plc].xp = data[plc * data_size + 8];

	}
}
void setSettings() {
	settings_of.close();
	settings_of.open(settings_path.c_str(), ios::in | ios::out);
	settings_of << PopUpPause << endl;
	settings_of << DataOnRight << endl;
	settings_of << stupid << endl;

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
	DataOnRight = settinhstab[1];
	stupid = settinhstab[2];

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
void drawItemUse(int pl, int sel) {
	system("cls");
	itemData iD = itemsData[check(players[pl].items[sel].name)];
	cout << "Position:" << sel << ". ";
	if (iD.stackable) {
		cout << players[pl].items[sel].number << "x ";
	}
	cout << iD.icon << " " << iD.name << ". Value: " << iD.value << ". ";
	switch (iD.Special_type)
	{
	case 0:
	{
		break;
	}
	case 1:
	{
		cout << "Melee damage: " << iD.Special_power;
		break;
	}
	case 2:
	{
		cout << "Ranged damage: " << iD.Special_power << ". (On start od combat +" << iD.Special_value << " turns)";
		break;
	}
	case 3:
	{
		cout << "Magic damage: " << iD.Special_power << ". (Armor penetration. Enemy has -" << iD.Special_value << " armor)";
		break;
	}
	default:
		break;
	}
	cout << endl;
	system("pause");
}
void drawInventory(int pl) {
	while (true) {
		system("cls");

		if (!stupid) {
			cout << "Player " << pl << "'s inventory" << endl;
		}
		else {
			cout << "Player " << pl + 1 << "'s inventory" << endl;

		}
		int n = players[pl].items.size();
		for (int i = 0; i < n; i++)
		{
			itemData iD = itemsData[check(players[pl].items[i].name)];
			cout << i << ": ";
			if (iD.stackable) {
				cout << players[pl].items[i].number << "x ";
			}
			cout << iD.icon << " " << iD.name << ". Value: " << iD.value;



			cout << endl;
		}
		if (!stupid) {
			cout << "'u' to open Player 0's inventory.'i' to open Player 1's inventory." << pl;
		}
		else {
			cout << "'u' to open Player 1's inventory.'i' to open Player 2's inventory." << pl;

		}
		cout << ".'x' to exit this menu.'o' to use item" << endl;


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
			it.name = "Rock";
			it.number = 1;
			addItem(it, pl);
			drawInventory(pl);


			break;
		}
		case 't': {
			item it;
			it.name = "Bow";
			it.number = 1;
			addItem(it, pl);
			drawInventory(pl);


			break;
		}
		case 'o': {
			cout << "Input Position of item. (first number)[if you input wrong character here it will break and to fix you need game]: "<<endl;
			int b = 0;
			cin >> b;
			if (b < players[pl].items.size()) {
				drawItemUse(pl, check(players[pl].items[b].name));
			}
			cin;
			break;
		}
		case 'x': {
			return;
		}
		default:
			return;
		}
	}
}

int main()
{
	srand(time(NULL));

	getPlayerStats();
	getItemsData();
	getInventory();
	getEnemiesData();
	getSettings();
	getMap();

	/*for (int x = 0; x < size_x; x++)
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
	}*/
	while (true) {

		drawMap(0);
		cout << endl;

		if (!DataOnRight) {
			for (int i = 0; i < 6; i++)
			{
				drawControls(i);
				cout << endl;
			}
		}
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
			if (!stupid) {
				cout << "Player " << 0 << ": x:" << players[0].player_x << "|y:" << players[0].player_y << endl;
				cout << "Player " << 1 << ": x:" << players[1].player_x << "|y:" << players[1].player_y << endl;
			}
			else {
				cout << "Player " << 1 << ": x:" << players[0].player_x << "|y:" << players[0].player_y << endl;
				cout << "Player " << 2 << ": x:" << players[1].player_x << "|y:" << players[1].player_y << endl;
			}


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
			getEnemiesData();
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
					case 'q':
					{
						PopUpPause = !PopUpPause;
						break;
					}

					case 'w':
					{
						DataOnRight = !DataOnRight;
						break;
					}case 'e':
					{
						stupid = !stupid;
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
		case 'c': {
			drawChars();

			break;
		}
		case 'j': {
			system("cls");
			for (int i = 0; i < Enemies.size(); i++)
			{
				cout << Enemies[i].name << endl;
				cout << Enemies[i].icon << endl;
				cout << Enemies[i].max_hp << endl;
				cout << Enemies[i].dmg << endl;
				cout << Enemies[i].hp_scal << endl;
				cout << Enemies[i].dmg_scal << endl;

			}
			system("pause");
			break;
		}

		}
		}
		for (int i = 0; i < 2; i++)
		{
			if (players[i].alive) {
				check_player_position(i);
			}
		}




	}

}
