#include <iostream>
#define SHIP_AMOUNT 10
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 21

using namespace std;

enum shipCategory {
	DES = 2,
	CRU,
	BAT,
	CAR
};
enum playerNumber {
	A = 0,
	B
};
enum direction {
	N = 0,
	E,
	S,
	W
};
struct ship {
	int isPlaced;
	int length;
	int direction;
	int x_position;
	int y_position;
	int classIterator;
};
struct player {
	int playersNumber;
	int fleetSize;
	int minBoardHeight;
	int maxBoardHeight;
	ship ships_array[SHIP_AMOUNT];
};
player initiatePlayer(int playerName);
ship createShip(int length, player* playerX, int iterator);
void setFleet(player* playerX, int a1, int a2, int a3, int a4);
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer);
void statePhase(player* playerArray[]);
void playerPhase(player* selectedPlayer);
void errorHandler(char tag[25], char text[50]);

int main() {
	char board[21][10];
	player playerA = initiatePlayer(A);
	player playerB = initiatePlayer(B);
	player* players[] = { &playerA, &playerB };
	char command[50];
	while (cin >> command) {
		if (command == "[state]") {
			statePhase(players);
		}
		if (command == "[playerA]") {
			playerPhase(players[A]);
		}
		if (command == "[playerB]") {
			playerPhase(players[B]);
		}
	}
	return 0;
}

void errorHandler(char tag[25], char text[50]) {
	cout << "INVALID OPERATION " << tag << " SPECIFIED: " << text;
}

void statePhase(player* playerArray[]) {
	char command[50];
	int end = 0;
	while (!end) {
		cin >> command;
		if (command == "[state]") {
			end = 1;
		}
		if (command == "SET_FLEET") {
			char buffor;
			player* selectedPlayer;
			int a1, a2, a3, a4;
			cin >> buffor;
			if (buffor == 'A') {
				selectedPlayer = playerArray[A];
			}
			else {
				selectedPlayer = playerArray[B];
			}
			cin >> a1 >> a2 >> a3 >> a4;
			setFleet(selectedPlayer, a1, a2, a3, a4);
		}
	}
}
void playerPhase(player* selectedPlayer) {
	char action[50];
	int end = 0;
	while (!end) {
		cin >> action;
		if (action == "[playerA]") {
			end = 1;
		}
		if (action == "PLACE_SHIP") {
			
		}
	}
}
player initiatePlayer(int playerName) {
	player playerDefault;
	playerDefault.playersNumber = playerName;
	playerDefault.fleetSize = 0;
	if (playerName == A) {
		playerDefault.minBoardHeight = 0;
		playerDefault.maxBoardHeight = 9;
	}
	else {
		playerDefault.minBoardHeight = 11;
		playerDefault.maxBoardHeight = 20;
	}
	return playerDefault;
}
ship createShip(int length, player* playerX, int iterator) {
	ship newShip;
	newShip.length = length + 2;
	newShip.isPlaced = 0;
	newShip.classIterator = iterator;
	return newShip;
}
void setFleet(player* playerX, int a1, int a2, int a3, int a4) {
	int fleetArray[] = {a1,a2,a3,a4};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < fleetArray[i]; j++) {
			int currentFleetSize = playerX->fleetSize;
			playerX->ships_array[currentFleetSize] = createShip(i,playerX,j);
			playerX->fleetSize++;
		}
	}
}
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer) {
	int fleetSize = currentPlayer->fleetSize;
	for (int i = 0; i < fleetSize; i++) {
		ship* currentShip = &(currentPlayer->ships_array[i]);
		if (currentPlayer->ships_array[i].isPlaced == 0) {
			if (currentPlayer->ships_array[i].length == shipClass && currentPlayer->ships_array[i].classIterator == iterator) {
				currentShip->isPlaced = 1;
				currentShip->direction = shipDirection;
				currentShip->x_position = positionX;
				currentShip->y_position = positionY;
			}
		}
		else {
			char tag[25] = "PLACE_SHIP";
			char text[50] = "SHIP ALREADY PRESENT";
			errorHandler(tag,text);
		}
	}
}
