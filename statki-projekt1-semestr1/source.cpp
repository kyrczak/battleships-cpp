#include <iostream>
#include <stdlib.h>
#include <string.h>

#define SHIP_AMOUNT 10
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

struct point;
struct ship;

struct point {
	int x;
	int y;
	char part;
	ship* whichShip;
};

struct ship {
	int isPlaced;
	unsigned int length;
	unsigned int direction;
	int classIterator;
	point** placement;
};
struct player {
	int playersNumber;
	int fleetSize;
	int remainingShipTiles;
	int fleetSet;
	int minBoardHeight;
	int minBoardWidth;
	int maxBoardHeight;
	int maxBoardWidth;
	ship ships_array[SHIP_AMOUNT];
};
struct game {
	int boardWidth;
	int boardHeight;
};

player initiatePlayer(int playerName);
ship createShip(int length, int iterator);
int shipClassOption(char buffor[5]);
int dirOption(char buffor);
void setFleet(player* playerX, int a1, int a2, int a3, int a4);
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board, char shipCHAR[5], char dirCHAR);
void statePlaceShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board,char playerCHAR, char shipCHAR[5], char dirCHAR, char* stateCHAR);
void statePhase(game* Game, player* playerArray[], point** board, int* nextPlayer);
void playerPhase(player* selectedPlayer, point** board, player* playerArray[], game* Game);
void shoot(int x, int y, point** board, player* playerArray[], game* Game);
void errorHandler(char tag[TAG_LENGTH], char text[TEXT_LENGTH]);
void print(game* Game, player* playerArray[], point** board, int x);
void reef(int y, int x, point** board, int maxX, int maxY);
point** boardPrep(point** array, int board_height, int board_width);
int checkShipPlacement(player* currentPlayer, int positionY, int positionX, int shipDirection, int shipClass);
int checkShipProximity(player* currentPlayer, point** board, int positionY, int positionX, int shipDirection, int shipClass);
int checkFleetPlace(player* playerArray[]);
int checkVictory(player* playerArray[]);

int main() {
	point** board = NULL;
	game Game;
	Game.boardHeight = 21;
	Game.boardWidth = 10;
	board = boardPrep(board,21, 10);
	player playerA = initiatePlayer(A);
	player playerB = initiatePlayer(B);
	player* players[] = { &playerA, &playerB };
	char command[TEXT_LENGTH];
	int nextPlayer = 0;
	while (cin>>command) {
		if (strcmp(command, "[state]")==0) {
			statePhase(&Game,players,board, &nextPlayer);
		}
		if (strcmp(command,"[playerA]")==0) {
			if (nextPlayer == 0) {
				playerPhase(players[A], board, players, &Game);
				nextPlayer = 1;
			}
			else {
				char tag[TAG_LENGTH] = "[playerA]";
				char text[TEXT_LENGTH] = "THE OTHER PLAYER EXPECTED";
				cout << "INVALID OPERATION \"" << tag << " \": " << text;
				exit(0);
			}
		}
		if (strcmp(command,"[playerB]")==0) {
			if (nextPlayer == 1) {
				playerPhase(players[B], board, players, &Game);
				nextPlayer = 0;
			}
			else {
				char tag[TAG_LENGTH] = "[playerB]";
				char text[TEXT_LENGTH] = "THE OTHER PLAYER EXPECTED";
				cout << "INVALID OPERATION \"" << tag << " \": " << text;
				exit(0);
			}
		}
	}
	return 0;
}

point** boardPrep(point** array, int board_height, int board_width) {
	array = (point **)malloc(board_height * sizeof(point*));
	for (int i = 0; i < board_height; i++) {
		array[i] = (point *)malloc(board_width * sizeof(point));
	}
	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_width; j++) {
			array[i][j].x = j;
			array[i][j].y = i;
			array[i][j].part = ' ';
			array[i][j].whichShip = NULL;
		}
	}
	return array;
}
void errorHandler(char tag[TAG_LENGTH], char text[TEXT_LENGTH]) {
	cout << "INVALID OPERATION " << tag << " SPECIFIED: " << text;
	exit(0);
}
void statePhase(game* Game, player* playerArray[], point** board, int* nextPlayer) {
	char command[TEXT_LENGTH];
	int end = 0;
	while (!end) {
		cin >> command;
		if (strcmp(command,"[state]")==0) {
			end = 1;
		}
		if (strcmp(command, "BOARD_SIZE") == 0) {
			int boardHeight, boardWidth;
			cin >> boardHeight >> boardWidth;
			for (int i = 0; i < Game->boardWidth; i++) {
				free(board[i]);
			}
			free(board);
			board = boardPrep(board, boardHeight, boardWidth);
			Game->boardHeight = boardHeight;
			Game->boardWidth = boardWidth;
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
			print(Game,playerArray, board, id);
		}
		if (strcmp(command, "NEXT_PLAYER") == 0) {
			char playerCHAR;
			cin >> playerCHAR;
			if (playerCHAR == 'A') {
				*nextPlayer = 0;
			}
			else {
				*nextPlayer = 1;
			}
		}
		if (strcmp(command, "INIT_POSITION") == 0) {
			char playerCHAR;
			int minX, minY, maxX, maxY;
			cin >> playerCHAR >> minY >> minX >> maxY >> maxX;
			if (playerCHAR == 'A') {
				playerArray[A]->minBoardHeight = minY;
				playerArray[A]->minBoardWidth = minX;
				playerArray[A]->maxBoardHeight = maxY;
				playerArray[A]->maxBoardWidth = maxX;
			}
			else {
				playerArray[B]->minBoardHeight = minY;
				playerArray[B]->minBoardWidth = minX;
				playerArray[B]->maxBoardHeight = maxY;
				playerArray[B]->maxBoardWidth = maxX;
			}
		}
		if (strcmp(command, "SHIP") == 0) {
			char playerCHAR, dirCHAR, classCHAR[5];
			int py, px, iterator;
			cin >> playerCHAR >> py >> px >> dirCHAR >> iterator >> classCHAR;
			int dirINT = dirOption(dirCHAR);
			int classINT = shipClassOption(classCHAR);
			char* stateCHAR = (char*)malloc((classINT+1) * sizeof(char));
			cin >> stateCHAR;
			if (playerCHAR == 'A') {
				statePlaceShip(py, px, dirINT, iterator, classINT, playerArray[A], board,playerCHAR, classCHAR, dirCHAR, stateCHAR);
			}
			else if(playerCHAR == 'B') {
				statePlaceShip(py, px, dirINT, iterator, classINT, playerArray[B], board,playerCHAR, classCHAR, dirCHAR, stateCHAR);
			}
			free(stateCHAR);
		}
		if (strcmp(command, "REEF") == 0) {
			int py, px;
			cin >> py >> px;
			reef(py, px, board,Game->boardWidth, Game->boardHeight);
		}
	}
}
void playerPhase(player* selectedPlayer, point** board, player* playerArray[], game* Game) {
	if (selectedPlayer->fleetSet == 0) {
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
			placeShip(posY, posX, INTdirection, iterator, INTshipClass, selectedPlayer, board, shipClass, direction);
		}
		if (strcmp(action, "SHOOT") == 0) {
			int x, y;
			cin >> y >> x;
			if (checkFleetPlace(playerArray) == 1) {
				shoot(x, y, board, playerArray,Game);
			}
			else {
				char tag[TAG_LENGTH] = "SHOOT";
				char text[TEXT_LENGTH] = "NOT ALL SHIPS PLACED";
				cout << "INVALID OPERATION \"" << tag << " "<<y << " " << x << "\": " << text;
				exit(0);
			}
		}
		switch (checkVictory(playerArray)) {
		case 0:
			break;
		case 1:
			cout << "B won" << endl;
			exit(0);
			break;
		case 2:
			cout << "A won" << endl;
			exit(0);
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
int checkFleetPlace(player* playerArray[]) {
	int allShipsPlaced = 1;
	for (int j = 0; j < PLAYER_AMOUNT; j++) {
		for (int i = 0; i < playerArray[j]->fleetSize; i++) {
			if (playerArray[j]->ships_array[i].isPlaced == 0) {
				allShipsPlaced = 0;
			}
		}
	}
	return allShipsPlaced;
}
int checkVictory(player* playerArray[]) {
	if (playerArray[0]->fleetSet == 1 && playerArray[1]->fleetSet == 1 && checkFleetPlace(playerArray) == 1) {
		if (playerArray[0]->remainingShipTiles == 0) {
			return 1;
		}
		else if (playerArray[1]->remainingShipTiles == 0) {
			return 2;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
	
}
int checkShipPlacement(player* currentPlayer, int positionY, int positionX, int shipDirection, int shipClass) {
	if (positionY <= currentPlayer->maxBoardHeight && positionY >= currentPlayer->minBoardHeight && positionX <= currentPlayer->maxBoardWidth && positionX >= currentPlayer->minBoardWidth) {
		if (shipDirection == N) {
			if ((positionY + shipClass - 1) > currentPlayer->maxBoardHeight) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (shipDirection == E) {
			if ((positionX - shipClass) >= currentPlayer->maxBoardWidth) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (shipDirection == W) {
			if ((positionX + shipClass) < currentPlayer->minBoardWidth) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (shipDirection == S) {
			if ((positionY - shipClass + 1) < currentPlayer->minBoardHeight) {
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
int checkShipProximity(player* currentPlayer, point** board, int positionY, int positionX, int shipDirection, int shipClass) {
	int result = 1;
	if (positionY > currentPlayer->maxBoardHeight || positionX > currentPlayer->maxBoardWidth)
		return 0;
	switch (shipDirection) {
	case N:
		for (int i = 0; i < shipClass; i++) {
			if (i == 0) {
				if ((positionY + i - 1) >= currentPlayer->minBoardHeight) {
					if(board[positionY + i - 1][positionX].part == '+')
					result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionY + i + 1) <= (currentPlayer->maxBoardHeight)) {
					if(board[positionY + i + 1][positionX].part == '+')
					result = 0;
				}
			}
			if ((positionX - 1) >= currentPlayer->minBoardWidth && (positionX + 1) <= currentPlayer->maxBoardWidth) {
				if(board[positionY + i][positionX - 1].part == '+'|| board[positionY + i][positionX + 1].part == '+')
				result = 0;
			}
		}
		break;
	case E:
		for (int i = 0; i < shipClass; i++) {
			if (i == 0) {
				if ((positionX - i + 1) <= currentPlayer->maxBoardWidth) {
					if(board[positionY][positionX - i + 1].part == '+')
					result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionX - i - 1) >= currentPlayer->minBoardWidth) {
					if(board[positionY][positionX - i - 1].part == '+')
					result = 0;
				}
			}
			if ((positionY - 1) >= currentPlayer->minBoardHeight && (positionY + 1) <= currentPlayer->maxBoardHeight) {
				if(board[positionY - 1][positionX - i].part == '+' || board[positionY + 1][positionX - i].part == '+')
				result = 0;
			}
		}
		break;
	case S:
		for (int i = 0; i < shipClass; i++) {
			if (i == 0) {
				if ((positionY - i + 1) <= currentPlayer->maxBoardHeight) {
					if(board[positionY - i + 1][positionX].part == '+')
					result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionY - 1) >= currentPlayer->minBoardHeight) {
					if(board[positionY - i - 1][positionX].part == '+')
					result = 0;
				}
			}
			if ( (positionX - 1) >= currentPlayer->minBoardWidth && (positionX + 1) <= currentPlayer->maxBoardWidth) {
				if(board[positionY - i][positionX - 1].part == '+' || board[positionY - i][positionX + 1].part == '+')
				result = 0;
			}
		}
		break;
	case W:
		for (int i = 0; i < shipClass; i++) {
			if (i == 0) {
				if ((positionX + i - 1) >= currentPlayer->minBoardWidth) {
					if(board[positionY][positionX + i - 1].part == '+')
					result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionX + i + 1) <= currentPlayer->maxBoardWidth) {
					if(board[positionY][positionX + i + 1].part == '+')
					result = 0;
				}
			}
			if ((positionY - 1) >= currentPlayer->minBoardHeight  && (positionY + 1) <= currentPlayer->maxBoardHeight) {
				if(board[positionY - 1][positionX + i].part == '+')
					result = 0;
				if (board[positionY + 1][positionX + i].part == '+')
					result = 0;
			}
		}
		break;
	default:
		result = 0;
		break;
	}
	return result;
}
int checkReefs(point** board, int positionY, int positionX, int shipDirection, int shipClass) {
	int result = 1;
	switch (shipDirection) {
	case N:
		for (int j = 0; j < (shipClass); j++) {
			point* placeOnBoard = &(board[positionY + j][positionX]);
			if (placeOnBoard->part == '#')
				result = 0;
		}
		break;
	case E:
		for (int j = 0; j < (shipClass); j++) {
			point* placeOnBoard = &(board[positionY][positionX - j]);
			if (placeOnBoard->part == '#')
				result = 0;
		}
		break;
	case S:
		for (int j = 0; j < (shipClass); j++) {
			point* placeOnBoard = &(board[positionY - j][positionX]);
			if (placeOnBoard->part == '#')
				result = 0;
		}
		break;
	case W:
		for (int j = 0; j < (shipClass); j++) {
			point* placeOnBoard = &(board[positionY][positionX + j]);
			if (placeOnBoard->part == '#')
				result = 0;
		}
		break;
	default:
		result = 0;
		break;
	}
	return result;
}

player initiatePlayer(int playerName)  {
	player playerDefault;
	playerDefault.playersNumber = playerName;
	playerDefault.fleetSize = 0;
	playerDefault.fleetSet = 0;
	playerDefault.remainingShipTiles = 0;
	if (playerName == A) {
		playerDefault.minBoardHeight = 0;
		playerDefault.maxBoardHeight = 9;
		playerDefault.minBoardWidth = 0;
		playerDefault.maxBoardWidth = 9;
	}
	else {
		playerDefault.minBoardHeight = 11;
		playerDefault.maxBoardHeight = 20;
		playerDefault.minBoardWidth = 0;
		playerDefault.maxBoardWidth = 9;
	}
	return playerDefault;
}
ship createShip(int length, int iterator) {
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
			playerX->ships_array[currentFleetSize] = createShip(i, j);
			playerX->fleetSize++;
		}
	}
	playerX->fleetSet = 1;
}
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board, char shipCHAR[5], char dirCHAR) {
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
				cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
				exit(0);
			}
		}
	}
	if (currentShip == NULL) {
		char text[TEXT_LENGTH] = "ALL SHIPS OF THE CLASS ALREADY SET";
		cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
		exit(0);
	}
	else if(checkShipPlacement(currentPlayer, positionY, positionX, shipDirection, shipClass) == 1){
		if(checkShipProximity(currentPlayer, board, positionY, positionX, shipDirection, shipClass) == 1) {
			if (checkReefs(board, positionY, positionX, shipDirection, shipClass) == 1) {
				switch (shipDirection) {
				case N:
					for (int j = 0; j < (currentShip->length); j++) {
						point* placeOnBoard = &(board[positionY + j][positionX]);
						placeOnBoard->whichShip = currentShip;
						placeOnBoard->part = '+';
						currentShip->placement[j] = placeOnBoard;
					}
					currentShip->isPlaced = 1;
					currentPlayer->remainingShipTiles += currentShip->length;
					break;
				case E:
					for (int j = 0; j < (currentShip->length); j++) {
						point* placeOnBoard = &(board[positionY][positionX - j]);
						placeOnBoard->whichShip = currentShip;
						placeOnBoard->part = '+';
						currentShip->placement[j] = placeOnBoard;
					}
					currentShip->isPlaced = 1;
					currentPlayer->remainingShipTiles += currentShip->length;
					break;
				case S:
					for (int j = 0; j < (currentShip->length); j++) {
						point* placeOnBoard = &(board[positionY - j][positionX]);
						placeOnBoard->whichShip = currentShip;
						placeOnBoard->part = '+';
						currentShip->placement[j] = placeOnBoard;
					}
					currentShip->isPlaced = 1;
					currentPlayer->remainingShipTiles += currentShip->length;
					break;
				case W:
					for (int j = 0; j < (currentShip->length); j++) {
						point* placeOnBoard = &(board[positionY][positionX + j]);
						placeOnBoard->whichShip = currentShip;
						placeOnBoard->part = '+';
						currentShip->placement[j] = placeOnBoard;
					}
					currentShip->isPlaced = 1;
					currentPlayer->remainingShipTiles += currentShip->length;
					break;
				}
			}
			else {
				char text[TEXT_LENGTH] = "PLACING SHIP ON REEF";
				cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
				exit(0);
			}

		}
		else {
			char text[TEXT_LENGTH] = "PLACING SHIP TOO CLOSE TO OTHER SHIP";
			cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
			exit(0);
		}
	}
	else {
		char text[TEXT_LENGTH] = "NOT IN STARTING POSITION";
		cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
		exit(0);
	}
}
void statePlaceShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board,char playerCHAR, char shipCHAR[5], char dirCHAR, char* stateCHAR) {
	if (currentPlayer->fleetSet == 0) {
		setFleet(currentPlayer, 1, 2, 3, 4);
	}
	char tag[TAG_LENGTH] = "SHIP";
	int fleetSize = currentPlayer->fleetSize;
	ship* currentShip = NULL;
	for (int i = 0; i < fleetSize; i++) {
		if (currentPlayer->ships_array[i].length == shipClass && currentPlayer->ships_array[i].classIterator == iterator) {
			if (currentPlayer->ships_array[i].isPlaced == 0) {
				currentShip = &(currentPlayer->ships_array[i]);
			}
			else {
				char text[TEXT_LENGTH] = "SHIP ALREADY PRESENT";
				//errorHandler(tag, text);
				cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
				exit(0);
			}
		}
	}
	if (currentShip == NULL) {
		char text[TEXT_LENGTH] = "ALL SHIPS OF THE CLASS ALREADY SET";
		cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
		exit(0);
	}
	else if(checkShipPlacement(currentPlayer, positionY, positionX, shipDirection, shipClass) == 1){
		if(checkShipProximity(currentPlayer, board, positionY, positionX, shipDirection, shipClass) == 1) {
			switch (shipDirection) {
			case N:
				for (int j = 0; j < (currentShip->length); j++) {
					point* placeOnBoard = &(board[positionY + j][positionX]);
					placeOnBoard->whichShip = currentShip;
					placeOnBoard->part = '+';
					currentShip->placement[j] = placeOnBoard;
				}
				currentShip->isPlaced = 1;
				currentPlayer->remainingShipTiles += currentShip->length;
				break;
			case E:
				for (int j = 0; j < (currentShip->length); j++) {
					point* placeOnBoard = &(board[positionY][positionX - j]);
					placeOnBoard->whichShip = currentShip;
					placeOnBoard->part = '+';
					currentShip->placement[j] = placeOnBoard;
				}
				currentShip->isPlaced = 1;
				currentPlayer->remainingShipTiles += currentShip->length;
				break;
			case S:
				for (int j = 0; j < (currentShip->length); j++) {
					point* placeOnBoard = &(board[positionY - j][positionX]);
					placeOnBoard->whichShip = currentShip;
					placeOnBoard->part = '+';
					currentShip->placement[j] = placeOnBoard;
				}
				currentShip->isPlaced = 1;
				currentPlayer->remainingShipTiles += currentShip->length;
				break;
			case W:
				for (int j = 0; j < (currentShip->length); j++) {
					point* placeOnBoard = &(board[positionY][positionX + j]);
					placeOnBoard->whichShip = currentShip;
					placeOnBoard->part = '+';
					currentShip->placement[j] = placeOnBoard;
				}
				currentShip->isPlaced = 1;
				currentPlayer->remainingShipTiles += currentShip->length;
				break;
			}

		}
		else {
			char text[TEXT_LENGTH] = "PLACING SHIP TOO CLOSE TO OTHER SHIP";
			cout << "INVALID OPERATION " << "\"" << tag << " " << playerCHAR << " " << positionY << " " << positionX<<" " << dirCHAR << " " << iterator << " " << shipCHAR <<" "<<stateCHAR<< "\": " << text << endl;
			exit(0);
		}
	}
	else {
		char text[TEXT_LENGTH] = "NOT IN STARTING POSITION";
		//errorHandler(tag, text);
		cout << "INVALID OPERATION " << "\"" << tag << " " << positionY << " " << positionX << " " << dirCHAR << " " << iterator << " " << shipCHAR << "\": " << text << endl;
		exit(0);
	}
	for (int i = 0; i < currentShip->length; i++) {
		if (stateCHAR[i] == '0') {
			currentShip->placement[i]->part = 'x';
			currentPlayer->remainingShipTiles--;
		}
	}
}
void shoot(int x, int y, point** board, player* playerArray[], game* Game) {
	if (y < Game->boardHeight || x < Game->boardWidth) {
		if (board[y][x].part == '+') {
			board[y][x].part = 'x';
			ship* shipShotAt = board[y][x].whichShip;
			for (int i = 0; i < PLAYER_AMOUNT; i++) {
				for (int j = 0; j < playerArray[i]->fleetSize; j++) {
					if (shipShotAt == &(playerArray[i]->ships_array[j])) {
						playerArray[i]->remainingShipTiles--;
					}
				}
			}
		}
	}
	else {
		char tag[TAG_LENGTH] = "SHOOT";
		char text[TEXT_LENGTH] = "FIELD DOES NOT EXIST";
		cout << "INVALID OPERATION \"" << tag << " " << y << " " << x << "\": " << text;
		exit(0);
	}
}
void print(game* Game, player* playerArray[], point** board, int x) {
	if (x == 0) {
		for (int i = 0; i < Game->boardHeight; i++) {
			for (int j = 0; j < Game->boardWidth; j++) {
				cout << board[i][j].part;
			}
			cout << endl;
		}
		cout << "PARTS REMAINING:: A : " << playerArray[A]->remainingShipTiles << " B : " << playerArray[B]->remainingShipTiles << endl;
	}
}
void reef(int y, int x, point** board, int maxX, int maxY) {
	if (y < maxY && y >= 0 && x < maxX && x >= 0) {
		board[y][x].part = '#';
	}
	else {
		cout << "INVALID OPERATION REEF " << y << " " << x << " SPECIFIED: REEF IS NOT PLACED ON BOARD";
		exit(0);
	}
}