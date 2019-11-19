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


//Static Data
int size_x = 20, size_y = 20;
char Map[20][20];
int start_x, start_y;
char inputer;
char player_input;
char inmovable[1] = { '#' };
HANDLE static hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


int randomize(int min, int max) {
	return rand() % (max - min + 1) + min;
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
				}else if (Map[x][y] == 't') {
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					cout << (char)207;
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
void drawStats() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "HP:" << hp << "/" << max_hp << " <3";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << " | ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Mana:" << mana << "/" << max_mana << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

}
void drawControls() {
	cout << "'wasd' to move.'p' to check player's position.'x' to quit." << endl;
}
int main()
{
	srand(time(NULL));
	string const Map_plik("map.txt");
	fstream plik(Map_plik.c_str(), ios::in | ios::out);
	ifstream plikif(Map_plik.c_str(), ios::in | ios::out);
	string nn = "";
	int a = 0;
	int b = 0;


	while (plik >> inputer) {

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
			cout << "x:" << player_x << "|y:" << player_y;
			_getch();
			break;
		}
		case 'x':
		{
			return 0;

			break;
		}
		default: {break; }
		}
	}
}
