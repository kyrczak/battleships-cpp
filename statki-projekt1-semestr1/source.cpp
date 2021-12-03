#include <iostream>
#include <stdlib.h>
#include <string.h>

#define SHIP_AMOUNT 10
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 21
#define TAG_LENGTH 25
#define TEXT_LENGTH 50
#define PLAYER_AMOUNT 2
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
	W,
	S
};

struct point {
	int x;
	int y;
	char part;
};
struct ship {
	int isPlaced;
	unsigned int length;
	unsigned int direction;
	int x_position;
	int y_position;
	int classIterator;
	point** placement;
};
struct player {
	int playersNumber;
	int fleetSize;
	int remainingShipTiles;
	int fleetPlaced;
	int minBoardHeight;
	int maxBoardHeight;
	ship ships_array[SHIP_AMOUNT];
};

player initiatePlayer(int playerName);
ship createShip(int length, player* playerX, int iterator);
int shipClassOption(char buffor[5]);
int dirOption(char buffor);
void setFleet(player* playerX, int a1, int a2, int a3, int a4);
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point(*board)[BOARD_HEIGHT][BOARD_WIDTH]);
void statePhase(player* playerArray[], point (*board)[BOARD_HEIGHT][BOARD_WIDTH]);
void playerPhase(player* selectedPlayer, point(*board)[BOARD_HEIGHT][BOARD_WIDTH]);
void errorHandler(char tag[TAG_LENGTH], char text[TEXT_LENGTH]);
void print(player* playerArray[], point (*board)[BOARD_HEIGHT][BOARD_WIDTH], int x);
void boardPrep(point(*array)[BOARD_HEIGHT][BOARD_WIDTH]);
int checkShipPlacement(player* currentPlayer, int positionY, int positionX, int shipDirection, int shipClass);

int main() {
	point board[BOARD_HEIGHT][BOARD_WIDTH];
	boardPrep(&board);
	player playerA = initiatePlayer(A);
	player playerB = initiatePlayer(B);
	player* players[] = { &playerA, &playerB };
	char command[50];
	while (cin >> command) {
		if (strcmp(command, "[state]")==0) {
			statePhase(players,&board);
		}
		if (strcmp(command,"[playerA]")==0) {
			playerPhase(players[A],&board);
		}
		if (strcmp(command,"[playerB]")==0) {
			playerPhase(players[B],&board);
		}
	}
	return 0;
}

void boardPrep(point (*array)[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			(*array)[i][j].x = j;
			(*array)[i][j].y = i;
			(*array)[i][j].part = ' ';
		}
	}
}
void errorHandler(char tag[TAG_LENGTH], char text[TEXT_LENGTH]) {
	cout << "INVALID OPERATION " << tag << " SPECIFIED: " << text;
	exit(0);
}
void statePhase(player* playerArray[], point (*board)[BOARD_HEIGHT][BOARD_WIDTH]) {
	char command[TEXT_LENGTH];
	int end = 0;
	while (!end) {
		cin >> command;
		if (strcmp(command,"[state]")==0) {
			end = 1;
		}
		if (strcmp(command,"SET_FLEET")==0) {
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
		if (strcmp(command,"PRINT")==0) {
			int id;
			cin >> id;
			print(playerArray, board, id);
		}
	}
}
void playerPhase(player* selectedPlayer, point (*board)[BOARD_HEIGHT][BOARD_WIDTH]) {
	if (selectedPlayer->fleetPlaced == 0) {
		setFleet(selectedPlayer, 1, 2, 3, 4);
	}
	char action[TEXT_LENGTH];
	int end = 0;
	while (!end) {
		cin >> action;
		if (strcmp(action,"[playerA]")==0 || strcmp(action,"[playerB]")==0) {
			end = 1;
		}
		if (strcmp(action,"PLACE_SHIP")==0) {
			int posY, posX, iterator;
			char direction;
			char shipClass[5];
			cin >> posY >> posX >> direction >> iterator >> shipClass;
			int INTdirection = dirOption(direction);
			int INTshipClass = shipClassOption(shipClass);
			placeShip(posY, posX, INTdirection, iterator, INTshipClass, selectedPlayer, board);
		}
	}
}
int shipClassOption(char buffor[5]) {
	if (strcmp(buffor, "DES") == 0) return DES;
	else if (strcmp(buffor, "CRU") == 0) return CRU;
	else if (strcmp(buffor, "BAT") == 0) return BAT;
	else if (strcmp(buffor, "CAR") == 0) return CAR;
	else return 0;
}
int dirOption(char buffor) {
	if (buffor == 'N') return N;
	else if (buffor == 'E') return E;
	else if (buffor == 'W') return W;
	else if (buffor == 'S') return S;
	else return 0;
}
player initiatePlayer(int playerName)  {
	player playerDefault;
	playerDefault.playersNumber = playerName;
	playerDefault.fleetSize = 0;
	playerDefault.fleetPlaced = 0;
	playerDefault.remainingShipTiles = 0;
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
	newShip.direction = 0;
	newShip.placement = (point **)malloc(sizeof(point*) * newShip.length);
	for (int i = 0; i < newShip.length; i++) {
		newShip.placement[i] = NULL;
	}
	return newShip;
}
void setFleet(player* playerX, int a1, int a2, int a3, int a4) {
	int fleetArray[] = {a1,a2,a3,a4};
	for (int i = 3, k=0; i >= 0; i--, k++) {
		for (int j = 0; j < fleetArray[k]; j++) {
			int currentFleetSize = playerX->fleetSize;
			playerX->ships_array[currentFleetSize] = createShip(i, playerX, j);
			playerX->remainingShipTiles += (i + 2);
			playerX->fleetSize++;
		}
	}
	playerX->fleetPlaced = 1;
}
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point (*board)[BOARD_HEIGHT][BOARD_WIDTH]) {
	char tag[TAG_LENGTH] = "PLACE_SHIP";
	int fleetSize = currentPlayer->fleetSize;
	ship* currentShip = NULL;
	for (int i = 0; i < fleetSize; i++) {
		if (currentPlayer->ships_array[i].length == shipClass && currentPlayer->ships_array[i].classIterator == iterator) {
			if (currentPlayer->ships_array[i].isPlaced == 0) {
				currentShip = &(currentPlayer->ships_array[i]);
			}
			else {
				char text[TEXT_LENGTH] = "SHIP ALREADY PRESENT";
				errorHandler(tag, text);
			}
		}
	}
	if (currentShip == NULL) {
		char text[TEXT_LENGTH] = "ALL SHIPS OF THE CLASS ALREADY SET";
		errorHandler(tag, text);
		//errorHandler "ALL SHIPS OF THE CLASS ALREADY SET"
	}
	else if(checkShipPlacement(currentPlayer,positionY,positionX,shipDirection,shipClass)==1) {
		switch (shipDirection) {
		case N:
			for (int j = 0; j < (currentShip->length); j++) {
				point* placeOnBoard = board[positionY + j][positionX];
				currentShip->placement[j] = placeOnBoard;
			}
			currentShip->isPlaced = 1;
			break;
		case E:
			for (int j = 0; j < (currentShip->length); j++) {
				currentShip->placement[j] = board[positionY][(positionX - j)];
				cout << currentShip->placement[j]->x << " : " << currentShip->placement[j]->y << endl;
			}
			currentShip->isPlaced = 1;
			break;
		case S:
			for (int j = 0; j < (currentShip->length); j++) {
				currentShip->placement[j] = board[positionY - j][positionX];
				cout << currentShip->placement[j]->x << " : " << currentShip->placement[j]->y << endl;
			}
			currentShip->isPlaced = 1;
			break;
		case W:
			for (int j = 0; j < (currentShip->length); j++) {
				currentShip->placement[j] = board[positionY][positionX + j];
				cout << currentShip->placement[j]->x << " : " << currentShip->placement[j]->y << endl;
			}
			currentShip->isPlaced = 1;
			break;
		}
	}
	else {
		char text[TEXT_LENGTH] = "NOT IN STARTING POSITION";
		errorHandler(tag, text);
	}
}
int checkShipPlacement(player* currentPlayer, int positionY, int positionX, int shipDirection, int shipClass) {
	if (positionY <= currentPlayer->maxBoardHeight && positionY >= currentPlayer->minBoardHeight) {
		if (shipDirection == N) {
			if ((positionY + shipClass-1) > currentPlayer->maxBoardHeight) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (shipDirection == E) {
			if ((positionX - shipClass) >= BOARD_WIDTH) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (shipDirection == W) {
			if ((positionX + shipClass) < 0) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (shipDirection == S) {
			if ((positionY - shipClass+1) < 0) {
				return 0;
			}
			else {
				return 1;
			}
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
void print(player* playerArray[], point (*board)[BOARD_HEIGHT][BOARD_WIDTH], int x) {
	if (x == 0) {
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			if (playerArray[i]->fleetSize > 0) {
				for (int j = 0; j < SHIP_AMOUNT; j++) {
					ship* currentShip = &(playerArray[i]->ships_array[j]);
					if (currentShip->isPlaced == 1) {
						for (int k = 0; k < (currentShip->length); k++) {
							currentShip->placement[k]->part = '+';
						}
					}
				}
			}
		}
		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				cout << (*board)[i][j].part;
			}
			cout << endl;
		}
		cout << "PARTS REMAINING:: A : " << playerArray[A]->remainingShipTiles << " B : " << playerArray[B]->remainingShipTiles << endl;
	}
}