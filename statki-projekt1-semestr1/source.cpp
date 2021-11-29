#include <iostream>
#define SHIP_AMOUNT 10
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 21

using namespace std;

enum ship_category {
	DES = 2,
	CRU,
	BAT,
	CAR
};
enum playerNumber {
	STATE = 0,
	A,
	B
};
enum direction {
	N,
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
	ship ships_array[SHIP_AMOUNT];
};
player initiatePlayer(int playerName);
ship createShip(int length, player* playerX, int iterator);
void setFleet(player* playerX, int a1, int a2, int a3, int a4);
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer);

int main() {
	char board[21][10];
	player playerA = initiatePlayer(A);
	player playerB = initiatePlayer(B);
	setFleet(&playerA, 1, 0, 0, 1);
	placeShip(4, 3, E, 0, DES, &playerA);
	return 0;
}
void playGame() {
	int currentPlayer = 0;
	int gameIsPlayed = 1;
}
player initiatePlayer(int playerName) {
	player playerDefault;
	playerDefault.playersNumber = playerName;
	playerDefault.fleetSize = 0;
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
		if (currentPlayer->ships_array[i].isPlaced == 0 && currentPlayer->ships_array[i].length == shipClass && currentPlayer->ships_array[i].classIterator == iterator) {
			currentShip->isPlaced = 1;
			currentShip->direction = shipDirection;
			currentShip->x_position = positionX;
			currentShip->y_position = positionY;
		}
	}
}