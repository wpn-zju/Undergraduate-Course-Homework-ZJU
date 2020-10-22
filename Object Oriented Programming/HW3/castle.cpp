#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "castle.h"

using namespace std;

Castle::Castle()
{
	char temp;
	cout << "Welcome to our game." << endl << "In this game, you need to find princess in the castle and then go back to the lobby." << endl << "Be aware of the monster!" << endl;
	cout << "Now we need to check for your settings which will affect the difficulty." << endl;

	cout << "Please set the size of your map(only odd number larger than or equal to 3)!" << endl;
	while (1) {
		cin >> scale;
		if (scale < 3 || scale % 2 == 0)
			cout << "illegal input, please input again" << endl;
		else
			break;
	}
	scale += 2;
	castleMap = new bool*[scale];
	rooms = new Room*[scale];
	for (int u = 0; u < scale; u++) {
		castleMap[u] = new bool[scale];
		rooms[u] = new Room[scale];
	}

	cout << "If you want to see the map, input 1." << endl;
	cout << "If not, input any other key." << endl;
	cin >> temp;
	if (temp == '1')
		mapVisible = 1;
	else
		mapVisible = 0;
	cout << "If you want to see the position of princess, input 1." << endl;
	cout << "If not, input any other key." << endl;
	cin >> temp;
	if (temp == '1')
		princessVisible = 1;
	else
		princessVisible = 0;
	cout << "If you want to see the your position every time, input 1." << endl;
	cout << "If not, input any other key." << endl;
	cin >> temp;
	if (temp == '1')
		myPositionVisible = 1;
	else
		myPositionVisible = 0;
	cout << "OK, let's start our game!" << endl;

	int i, j, k;
	srand(time(NULL));
	for (i = 0; i < scale; i++) {
		for (j = 0; j < scale; j++) {
			castleMap[i][j] = 0;
			for (k = 0; k < 4; k++) {
				rooms[i][j].exits[k] = 0;
				rooms[i][j].name = " ";
			}
		}
	}
	roomNum = 0;
	for (i = 1; i < scale - 1; i++) {
		for (j = 1; j < scale - 1; j++) {
			k = rand() % 4;
			castleMap[i][j] = (k == 0 || k == 1 || k == 2);
			if (castleMap[i][j])
				roomNum++;
		}
	}
	while (1) {
		i = rand() % (scale - 2);
		j = rand() % (scale - 2);
		if (castleMap[i][j] && (i != scale / 2 || j != scale / 2)) {
			distinyx = i;
			distinyy = j;
			break;
		}
	}
	castleMap[scale / 2][scale / 2] = 1;
	castleMap[scale / 2][scale / 2 - 1] = 1;
	castleMap[scale / 2][scale / 2 + 1] = 1;
	castleMap[scale / 2 - 1][scale / 2] = 1;
	castleMap[scale / 2 + 1][scale / 2] = 1;
	if (mapVisible) {
		cout << "Here is the map!" << endl;
		for (i = 0; i < scale; i++) {
			for (j = 0; j < scale; j++) {
				cout << castleMap[i][j] << " ";
			}
			cout << endl;
		}
	}
	if (princessVisible)
		cout << "Princess's position is " << "(" << distinyx << "," << distinyy << "). Find her now!" << endl;
	getPrincess = 0;
	for (i = 1; i < scale - 1; i++) {
		for (j = 1; j < scale - 1; j++) {
			if (castleMap[i][j]) {
				if (castleMap[i + 1][j]) {
					rooms[i][j].exits[1] = 1;
				}
				if (castleMap[i - 1][j]) {
					rooms[i][j].exits[3] = 1;
				}
				if (castleMap[i][j + 1]) {
					rooms[i][j].exits[0] = 1;
				}
				if (castleMap[i][j - 1]) {
					rooms[i][j].exits[2] = 1;
				}
			}
		}
	}
	x = scale / 2;
	y = scale / 2;
	rooms[scale / 2][scale / 2].name = "lobby";
	gameOver = 0;
}

Castle::~Castle()
{
	for (int i = 0; i < scale; i++) {
		delete[]rooms[i];
		delete[]castleMap[i];
	}
	delete[]rooms;
	delete[]castleMap;
}

void Castle::run()
{
	string dir;
	while (1) {
		if (rooms[x][y].name == " ") {
			cout << "Input new room's name!" << endl;
			cin >> rooms[x][y].name;
		}
		currentRoom = rooms[x][y];
		cout << "You enter room " << currentRoom.name << "!" << endl;
		if (myPositionVisible)
			cout << "Now your our position is " << "(" << x << "," << y << ")." << endl;
		if (x == distinyx && y == distinyy && !getPrincess) {
			cout << "You meet princess, now you could return to the lobby!" << endl;
			getPrincess = 1;
		}
		if (getPrincess && x == scale / 2 && y == scale / 2) {
			cout << "You Win!" << endl;
			return;
		}
		cout << "Select your moving direction:";
		if (currentRoom.exits[0])
			cout << " east ";
		if (currentRoom.exits[1])
			cout << " south ";
		if (currentRoom.exits[2])
			cout << " west ";
		if (currentRoom.exits[3])
			cout << " north ";
		cout << "." << endl;
		cin >> dir;
		if (dir == "east") {
			if (currentRoom.exits[0]) {
				y++;
				if (checkMonster())
					gameover();
			}
			else
				cout << "illegal direction" << endl;
		}
		else if (dir == "west") {
			if (currentRoom.exits[2]) {
				y--;
				if (checkMonster())
					gameover();
			}
			else
				cout << "illegal direction" << endl;
		}
		else if (dir == "north") {
			if (currentRoom.exits[3]) {
				x--;
				if (checkMonster())
					gameover();
			}
			else
				cout << "illegal direction" << endl;
		}
		else if (dir == "south") {
			if (currentRoom.exits[1]) {
				x++;
				if (checkMonster())
					gameover();
			}
			else
				cout << "illegal direction" << endl;
		}
		else
			cout << "illegal direction" << endl;
		if (gameOver)
			return;
	}
}

bool Castle::checkMonster()
{
	srand(time(NULL));
	return (0 == (rand() % roomNum));
}

void Castle::gameover()
{
	cout << "You meet monster! Game Over!" << endl;
	gameOver = 1;
}
