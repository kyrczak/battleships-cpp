#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
	int length;
	int direction;
	int numberOfMoves;
	int numberOfShots;
	int classIterator;
	int radarPosition;
	int isRadarWorking;
	int enginePosition;
	int isEngineWorking;
	int cannonPosition;
	int isCannonWorking;
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
	int isExtended;
};

player initiatePlayer(int playerName);
ship createShip(int length, int iterator);
point** boardPrep(int board_height, int board_width);
void setFleet(player* playerX, int a1, int a2, int a3, int a4);
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char shipCHAR[5], char dirCHAR);
void statePlaceShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board,game* Game, char playerCHAR, char shipCHAR[5], char dirCHAR, char* stateCHAR);
void statePhase(game* Game, player* playerArray[], point*** board, int* nextPlayer);
void playerPhase(player* selectedPlayer, point** board, player* playerArray[], game* Game);
void shoot(int x, int y, point** board, player* playerArray[], game* Game);
void print(game* Game, player* playerArray[], point** board, int x);
void reef(int y, int x, point** board, int maxX, int maxY);
void shipMove(int iterator, int shipClass, char moveDirection, player* currentPlayer, point** board, game* Game, char shipCHAR[5]);
void shipMoveForward(int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char tag[TAG_LENGTH], char shipCHAR[5], char moveDirection);
void shipMoveLeft(int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char tag[TAG_LENGTH], char shipCHAR[5], char moveDirection);
void shipMoveRight(int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char tag[TAG_LENGTH], char shipCHAR[5], char moveDirection);
void refreshMoves(player* currentPlayer);
void printExtended(player* playerArray[], point** board, int x, int y);
void shootExtended(player* currentPlayer, player* playerArray[], point** board, game* Game, int iterator, int posY, int posX, int shipClass, char shipCHAR[5]);
int checkShipPlacement(player* currentPlayer, int positionY, int positionX, int shipDirection, int shipClass);
int checkShipProximity(player* currentPlayer, point** board, game* Game, int positionY, int positionX, int shipDirection, int shipClass);
int moveCheckProximity(ship* currentShip, point** board, game* Game, int shipDirection, int shipClass);
int checkFleetPlace(player* playerArray[]);
int checkVictory(player* playerArray[]);
int shipClassOption(char buffor[5]);
int dirOption(char buffor);
int shootDistance(int posX, int posY, ship* currentShip);

int main() {
	point** board = NULL;
	game Game{};
	Game.boardHeight = 21;
	Game.boardWidth = 10;
	Game.isExtended = 0;
	board = boardPrep(21, 10);
	player playerA = initiatePlayer(A);
	player playerB = initiatePlayer(B);
	player* players[] = { &playerA, &playerB };
	char command[TEXT_LENGTH];
	int nextPlayer = 0;
	while (cin>>command) {
		if (strcmp(command, "[state]")==0) {
			statePhase(&Game,players,&board, &nextPlayer);
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

point** boardPrep(int board_height, int board_width) {
	point** array = new point*[board_height];
	for (int i = 0; i < board_height; i++) {
		array[i] = new point[board_width];
	}
	if (array != NULL) {
		for (int i = 0; i < board_height; i++) {
			for (int j = 0; j < board_width; j++) {
				array[i][j].x = j;
				array[i][j].y = i;
				array[i][j].part = ' ';
				array[i][j].whichShip = NULL;
			}
		}
	}
	return array;
}
void statePhase(game* Game, player* playerArray[], point*** board, int* nextPlayer) {
	char command[TEXT_LENGTH];
	int end = 0;
	while (!end) {
		cin >> command;
		if (strcmp(command,"[state]")==0) {
			end = 1;
		}
		if (strcmp(command, "BOARD_SIZE") == 0) {
			for (int i = 0; i < Game->boardWidth; i++) {
				delete[] (* board)[i];
			}
			delete[] (* board);
			int boardHeight, boardWidth;
			cin >> boardHeight >> boardWidth;
			*board = boardPrep(boardHeight, boardWidth);
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
			print(Game,playerArray, *board, id);
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
			char* stateCHAR = new char[classINT + 1];
			//char* stateCHAR = (char*)malloc((classINT+1) * sizeof(char));
			cin >> stateCHAR;
			if (playerCHAR == 'A') {
				statePlaceShip(py, px, dirINT, iterator, classINT, playerArray[A], *board, Game, playerCHAR, classCHAR, dirCHAR, stateCHAR);
			}
			else if(playerCHAR == 'B') {
				statePlaceShip(py, px, dirINT, iterator, classINT, playerArray[B], *board, Game, playerCHAR, classCHAR, dirCHAR, stateCHAR);
			}
			delete[] stateCHAR;
			//free(stateCHAR);
		}
		if (strcmp(command, "REEF") == 0) {
			int py, px;
			cin >> py >> px;
			reef(py, px, *board,Game->boardWidth, Game->boardHeight);
		}
		if (strcmp(command, "EXTENDED_SHIPS") == 0) {
			Game->isExtended = 1;
		}
	}
}
void playerPhase(player* selectedPlayer, point** board, player* playerArray[], game* Game) {
	if (selectedPlayer->fleetSet == 0) {
		setFleet(selectedPlayer, 1, 2, 3, 4);
	}
	char action[TEXT_LENGTH];
	int end = 0;
	refreshMoves(selectedPlayer);
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
			placeShip(posY, posX, INTdirection, iterator, INTshipClass, selectedPlayer, board, Game, shipClass, direction);
		}
		if (strcmp(action, "SHOOT") == 0 && Game->isExtended == 0) {
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
		if (strcmp(action, "SHOOT") == 0 && Game->isExtended == 1) {
			int iterator, posY, posX;
			char shipClass[5];
			cin >> iterator >> shipClass >> posY >> posX;
			int classINT = shipClassOption(shipClass);
			shootExtended(selectedPlayer,playerArray, board, Game, iterator, posY, posX, classINT,shipClass);
		}
		if (strcmp(action, "MOVE") == 0) {
			int iterator;
			char classCHAR[5];
			char moveDirection;
			cin >> iterator >> classCHAR >> moveDirection;
			int shipClass = shipClassOption(classCHAR);
			shipMove(iterator, shipClass, moveDirection, selectedPlayer, board, Game,classCHAR);
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
int checkShipProximity(player* currentPlayer, point** board, game* Game, int positionY, int positionX, int shipDirection, int shipClass) {
	int result = 1;
	if (positionY > Game->boardHeight|| positionX > Game->boardWidth)
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
			if (i == (shipClass - i - 1)) {
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
			if (board[positionY + j][positionX].part == '#')
				result = 0;
		}
		break;
	case E:
		for (int j = 0; j < (shipClass); j++) {
			if (board[positionY][positionX - j].part == '#')
				result = 0;
		}
		break;
	case S:
		for (int j = 0; j < (shipClass); j++) {
			if (board[positionY - j][positionX].part == '#')
				result = 0;
		}
		break;
	case W:
		for (int j = 0; j < (shipClass); j++) {
			if (board[positionY][positionX + j].part == '#')
				result = 0;
		}
		break;
	default:
		result = 0;
		break;
	}
	return result;
}
int moveCheckProximity(ship* currentShip, point** board, game* Game, int shipDirection, int shipClass) {
	int result = 1, positionY, positionX;
	switch (shipDirection) {
	case N:
		for (int i = 0; i < shipClass; i++) {
			positionY = currentShip->placement[i]->y;
			positionX = currentShip->placement[i]->x;
			if (i == 0) {
				if ((positionY + i - 1) >= 0) {
					if (board[positionY + i - 1][positionX].part == '+')
						result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionY + i + 1) < Game->boardHeight) {
					if (board[positionY + i + 1][positionX].part == '+')
						result = 0;
				}
			}
			if ((positionX - 1) >= 0 && (positionX + 1) < Game->boardWidth) {
				if (board[positionY + i][positionX - 1].part == '+' || board[positionY + i][positionX + 1].part == '+')
					result = 0;
			}
		}
		break;
	case E:
		for (int i = 0; i < shipClass; i++) {
			positionY = currentShip->placement[i]->y;
			positionX = currentShip->placement[i]->x;
			if (i == 0) {
				if ((positionX - i + 1) < Game->boardWidth) {
					if (board[positionY][positionX - i + 1].part == '+')
						result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionX - i - 1) >= 0) {
					if (board[positionY][positionX - i - 1].part == '+')
						result = 0;
				}
			}
			if ((positionY - 1) >= 0 && (positionY + 1) < Game->boardHeight) {
				if (board[positionY - 1][positionX - i].part == '+' || board[positionY + 1][positionX - i].part == '+')
					result = 0;
			}
		}
		break;
	case S:
		for (int i = 0; i < shipClass; i++) {
			positionY = currentShip->placement[i]->y;
			positionX = currentShip->placement[i]->x;
			if (i == 0) {
				if ((positionY - i + 1) < Game->boardHeight) {
					if (board[positionY - i + 1][positionX].part == '+')
						result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionY -i- 1) >= 0) {
					if (board[positionY - i - 1][positionX].part == '+')
						result = 0;
				}
			}
			if ((positionX - 1) >= 0 && (positionX + 1) < Game->boardWidth) {
				if (board[positionY - i][positionX - 1].part == '+' || board[positionY - i][positionX + 1].part == '+')
					result = 0;
			}
		}
		break;
	case W:
		for (int i = 0; i < shipClass; i++) {
			positionY = currentShip->placement[i]->y;
			positionX = currentShip->placement[i]->x;
			if (i == 0) {
				if ((positionX + i - 1) >= 0) {
					if (board[positionY][positionX + i - 1].part == '+')
						result = 0;
				}
			}
			if (i == (shipClass - 1)) {
				if ((positionX + i + 1) < Game->boardWidth) {
					if (board[positionY][positionX + i + 1].part == '+')
						result = 0;
				}
			}
			if ((positionY - 1) >= 0 && (positionY + 1) < Game->boardHeight) {
				if (board[positionY - 1][positionX + i].part == '+')
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
int shootDistance(int posX, int posY, ship* currentShip) {
	if (currentShip->length == CAR) {
		return 0;
	}
	else {
		int cannonPosition = currentShip->cannonPosition;
		int shipX = currentShip->placement[cannonPosition]->x;
		int shipY = currentShip->placement[cannonPosition]->y;
		return (sqrt(abs(shipY - posY) * abs(shipY - posY) + abs(shipX - posX) * abs(shipX - posX)));
	}
}

player initiatePlayer(int playerName)  {
	player playerDefault{};
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
	ship newShip{};
	newShip.length = length + 2;
	newShip.isPlaced = 0;
	newShip.classIterator = iterator;
	newShip.direction = 0;
	newShip.numberOfMoves = 0;
	newShip.numberOfShots = 0;
	newShip.radarPosition = 0;
	newShip.cannonPosition = 1;
	newShip.isCannonWorking = 1;
	newShip.isEngineWorking = 1;
	newShip.isRadarWorking = 1;
	newShip.enginePosition = newShip.length - 1;
	newShip.placement = new point * [newShip.length];
	//newShip.placement = (point **)malloc(sizeof(point*) * newShip.length);
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
void placeShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board,game* Game, char shipCHAR[5], char dirCHAR) {
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
		if(checkShipProximity(currentPlayer, board, Game , positionY, positionX, shipDirection, shipClass) == 1) {
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
					currentShip->direction = shipDirection;
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
					currentShip->direction = shipDirection;
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
					currentShip->direction = shipDirection;
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
					currentShip->direction = shipDirection;
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
void statePlaceShip(int positionY, int positionX, int shipDirection, int iterator, int shipClass, player* currentPlayer, point** board, game* Game,char playerCHAR, char shipCHAR[5], char dirCHAR, char* stateCHAR) {
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
		if(checkShipProximity(currentPlayer, board, Game, positionY, positionX, shipDirection, shipClass) == 1) {
			switch (shipDirection) {
			case N:
				for (int j = 0; j < (currentShip->length); j++) {
					point* placeOnBoard = &(board[positionY + j][positionX]);
					placeOnBoard->whichShip = currentShip;
					placeOnBoard->part = '+';
					currentShip->placement[j] = placeOnBoard;
				}
				currentShip->isPlaced = 1;
				currentShip->direction = shipDirection;
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
				currentShip->direction = shipDirection;
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
				currentShip->direction = shipDirection;
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
				currentShip->direction = shipDirection;
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
	if (x == 1) {
		if (Game->boardWidth > 10) {
			for (int i = 0; i < Game->boardWidth + 2; i++) {
				if (i - 2 < 0)
					cout << ' ';
				else {
					cout << ((i - 2) / 10);
				}
			}
			cout << endl;
			for (int i = 0; i < Game->boardWidth + 2; i++) {
				if (i - 2 < 0)
					cout << ' ';
				else {
					cout << ((i - 2) % 10);
				}
			}
			cout << endl;
		}
		else {
			for (int i = 0; i < Game->boardWidth + 2; i++) {
				if (i - 2 < 0)
					cout << ' ';
				else {
					cout << ((i - 2) % 10);
				}
			}
			cout << endl;
		}
		for (int i = 0; i < Game->boardHeight; i++) {
			for (int j = 0; j < Game->boardWidth + 2; j++) {
				if (j == 0)
					cout << (i / 10);
				else if (j == 1)
					cout << (i % 10);
				else
					printExtended(playerArray, board, (j - 2), i);
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
void shipMove(int iterator, int shipClass, char moveDirection, player* currentPlayer, point** board, game* Game, char shipCHAR[5]) {
	char tag[TAG_LENGTH] = "MOVE";
	ship* currentShip = NULL;
	for (int i = 0; i < currentPlayer->fleetSize; i++) {
		if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
			currentShip = &(currentPlayer->ships_array[i]);
	}
	if (currentShip->numberOfMoves == 0) {
		char text[TEXT_LENGTH] = "SHIP MOVED ALREADY";
		cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
		exit(0);
	}
	if (moveDirection == 'F') {
		shipMoveForward(iterator, shipClass, currentPlayer, board, Game,tag,shipCHAR,moveDirection);
		currentShip = NULL;
		for (int i = 0; i < currentPlayer->fleetSize; i++) {
			if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
				currentShip = &(currentPlayer->ships_array[i]);
		}
		currentShip->numberOfMoves--;
		if (moveCheckProximity(currentShip,board,Game,currentShip->direction,shipClass)==0) {
			char text[TEXT_LENGTH] = "PLACING SHIP TOO CLOSE TO OTHER SHIP";
			cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection<< "\": " << text << endl;
			exit(0);
		}
	}
	if (moveDirection == 'L') {
		shipMoveForward(iterator, shipClass, currentPlayer, board, Game, tag, shipCHAR, moveDirection);
		shipMoveLeft(iterator, shipClass, currentPlayer, board, Game, tag, shipCHAR, moveDirection);
		currentShip = NULL;
		for (int i = 0; i < currentPlayer->fleetSize; i++) {
			if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
				currentShip = &(currentPlayer->ships_array[i]);
		}
		currentShip->numberOfMoves--;
		if (moveCheckProximity(currentShip, board, Game, currentShip->direction, shipClass) == 0) {
			char text[TEXT_LENGTH] = "PLACING SHIP TOO CLOSE TO OTHER SHIP";
			cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
			exit(0);
		}

	}
	if (moveDirection == 'R') {
		shipMoveForward(iterator, shipClass, currentPlayer, board, Game, tag, shipCHAR, moveDirection);
		shipMoveRight(iterator, shipClass, currentPlayer, board, Game, tag, shipCHAR, moveDirection);
		currentShip = NULL;
		for (int i = 0; i < currentPlayer->fleetSize; i++) {
			if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
				currentShip = &(currentPlayer->ships_array[i]);
		}
		currentShip->numberOfMoves--;
		if (moveCheckProximity(currentShip, board, Game, currentShip->direction, shipClass) == 0) {
			char text[TEXT_LENGTH] = "PLACING SHIP TOO CLOSE TO OTHER SHIP";
			cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
			exit(0);
		}
	}
}
void shipMoveForward(int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char tag[TAG_LENGTH], char shipCHAR[5], char moveDirection) {
	ship* currentShip = NULL;
	for (int i = 0; i < currentPlayer->fleetSize; i++) {
		if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
			currentShip = &(currentPlayer->ships_array[i]);
	}
	if (currentShip != NULL) {
		switch (currentShip->direction) {
		case N:
			for (int i = 0; i < currentShip->length; i++) {
				if ((currentShip->placement[i]->y - 1) >= 0) {
					point tempPoint = board[currentShip->placement[i]->y - 1][currentShip->placement[i]->x];
					if (tempPoint.part == '#') {
						char text[TEXT_LENGTH] = "PLACING SHIP ON REEF";
						cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
						exit(0);
					}
					point* nextPoint = &(board[currentShip->placement[i]->y - 1][currentShip->placement[i]->x]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
				}
				else {
					char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
					cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
					exit(0);
				}
			}
			break;
		case E:
			for (int i = 0; i < currentShip->length; i++) {
				if ((currentShip->placement[i]->x + 1) <= Game->boardWidth) {
					point tempPoint = board[currentShip->placement[i]->y][currentShip->placement[i]->x + 1];
					point* nextPoint = &(board[currentShip->placement[i]->y][currentShip->placement[i]->x + 1]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
				}
				else {
					char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
					cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
					exit(0);
				}
			}
			break;
		case S:
			for (int i = 0; i < currentShip->length; i++) {
				if ((currentShip->placement[i]->y + 1) <= Game->boardHeight) {
					point tempPoint = board[currentShip->placement[i]->y + 1][currentShip->placement[i]->x];
					point* nextPoint = &(board[currentShip->placement[i]->y + 1][currentShip->placement[i]->x]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
				}
				else {
					char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
					cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
					exit(0);
				}
			}
			break;
		case W:
			for (int i = 0; i < currentShip->length; i++) {
				if ((currentShip->placement[i]->x - 1) >= 0) {
					point tempPoint = board[currentShip->placement[i]->y][currentShip->placement[i]->x - 1];
					point* nextPoint = &(board[currentShip->placement[i]->y][currentShip->placement[i]->x - 1]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
				}
				else {
					char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
					cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
					exit(0);
				}
			}
			break;
		}
	}
}
void shipMoveLeft(int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char tag[TAG_LENGTH], char shipCHAR[5], char moveDirection) {
	ship* currentShip = NULL;
	for (int i = 0; i < currentPlayer->fleetSize; i++) {
		if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
			currentShip = &(currentPlayer->ships_array[i]);
	}
	if (currentShip != NULL) {
		int stablePosX = 0;
		int stablePosY = 0;
		switch (currentShip->direction) {
		case N:
			 stablePosX = currentShip->placement[0]->x;
			 stablePosY = currentShip->placement[0]->y;
			if ((stablePosX - (currentShip->length - 1)) >= 0) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY][stablePosX - k];
					point* nextPoint = &(board[stablePosY][stablePosX - k]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = W;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		case E:
			 stablePosX = currentShip->placement[0]->x;
			 stablePosY = currentShip->placement[0]->y;
			if ((stablePosY - (currentShip->length - 1)) >= 0) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY-k][stablePosX];
					point* nextPoint = &(board[stablePosY-k][stablePosX]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = N;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		case S:
			 stablePosX = currentShip->placement[0]->x;
			 stablePosY = currentShip->placement[0]->y;
			if ((stablePosX + (currentShip->length - 1)) < Game->boardHeight) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY][stablePosX + k];
					point* nextPoint = &(board[stablePosY][stablePosX + k]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = E;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		case W:
			 stablePosX = currentShip->placement[0]->x;
			 stablePosY = currentShip->placement[0]->y;
			if ((stablePosY + (currentShip->length - 1)) >= 0) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY + k][stablePosX];
					point* nextPoint = &(board[stablePosY + k][stablePosX]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = S;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		}
	}
}
void shipMoveRight(int iterator, int shipClass, player* currentPlayer, point** board, game* Game, char tag[TAG_LENGTH], char shipCHAR[5], char moveDirection) {
	ship* currentShip = NULL;
	for (int i = 0; i < currentPlayer->fleetSize; i++) {
		if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
			currentShip = &(currentPlayer->ships_array[i]);
	}
	if (currentShip != NULL) {
		int stablePosX = 0;
		int stablePosY = 0;
		switch (currentShip->direction) {
		case N:
			stablePosX = currentShip->placement[0]->x;
			stablePosY = currentShip->placement[0]->y;
			if ((stablePosX + (currentShip->length - 1)) < Game->boardHeight) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY][stablePosX + k];
					point* nextPoint = &(board[stablePosY][stablePosX + k]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = E;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		case E:
			stablePosX = currentShip->placement[0]->x;
			stablePosY = currentShip->placement[0]->y;
			if ((stablePosY + (currentShip->length - 1)) >= 0) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY + k][stablePosX];
					point* nextPoint = &(board[stablePosY + k][stablePosX]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = S;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		case S:
			stablePosX = currentShip->placement[0]->x;
			stablePosY = currentShip->placement[0]->y;
			if ((stablePosX - (currentShip->length - 1)) >= 0) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY][stablePosX - k];
					point* nextPoint = &(board[stablePosY][stablePosX - k]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = W;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;

		case W:
			stablePosX = currentShip->placement[0]->x;
			stablePosY = currentShip->placement[0]->y;
			if ((stablePosY - (currentShip->length - 1)) >= 0) {
				for (int i = 0, k = (currentShip->length - 1); i < currentShip->length; i++, k--) {
					point tempPoint = board[stablePosY - k][stablePosX];
					point* nextPoint = &(board[stablePosY - k][stablePosX]);
					nextPoint->part = currentShip->placement[i]->part;
					nextPoint->whichShip = currentShip->placement[i]->whichShip;
					currentShip->placement[i]->part = tempPoint.part;
					currentShip->placement[i]->whichShip = tempPoint.whichShip;
					currentShip->placement[i] = nextPoint;
					currentShip->direction = N;
				}
			}
			else {
				char text[TEXT_LENGTH] = "SHIP WENT FROM BOARD";
				cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << moveDirection << "\": " << text << endl;
				exit(0);
			}
			break;
		}
	}
}
void refreshMoves(player* currentPlayer) {
	for (int i = 0; i < currentPlayer->fleetSize; i++) {
		if (currentPlayer->ships_array[i].length == CAR) {
			currentPlayer->ships_array[i].numberOfMoves = 2;
			currentPlayer->ships_array[i].numberOfShots = CAR;
		}
		else {
			currentPlayer->ships_array[i].numberOfMoves = 3;
			currentPlayer->ships_array[i].numberOfShots = currentPlayer->ships_array[i].length;
		}
	}
	
}	
void printExtended(player* playerArray[], point** board, int x, int y) {
	if (board[y][x].part == ' ' || board[y][x].part == '#' || board[y][x].part=='x')
		cout << board[y][x].part;
	else if (board[y][x].part == '+') {
		ship* printedShip = board[y][x].whichShip;
		int whichPartOfTheShip = 0;
		for (int i = 0; i < printedShip->length; i++) {
			if (printedShip->placement[i]->x == x && printedShip->placement[i]->y == y) {
				whichPartOfTheShip = i;
			}
		}
		if (whichPartOfTheShip == printedShip->radarPosition) {
			cout << '@';
		}
		else if (whichPartOfTheShip == printedShip->enginePosition) {
			cout << '%';
		}
		else if (whichPartOfTheShip == printedShip->cannonPosition) {
			if (printedShip->cannonPosition == printedShip->enginePosition) {
				cout << '%';
			}
			else {
				cout << '!';
			}
		}
		else {
			cout << '+';
		}
	}
}
void shootExtended(player* currentPlayer, player* playerArray[], point** board, game* Game, int iterator, int posY, int posX, int shipClass, char shipCHAR[5]) {
	char tag[TAG_LENGTH] = "SHOOT";
	player* playerShotAt = NULL;
	ship* currentShip = NULL;
	ship* shipShotAt = NULL;
	int placeShotAt = 0;
	for (int i = 0; i < currentPlayer->fleetSize; i++) {
		if (currentPlayer->ships_array[i].classIterator == iterator && currentPlayer->ships_array[i].length == shipClass)
			currentShip = &(currentPlayer->ships_array[i]);
	}
	if (currentShip->numberOfShots> 0) {
		if (currentShip->isCannonWorking == 1) {
			if (posY < Game->boardHeight && posX < Game->boardWidth) {
				if (shootDistance(posX, posY, currentShip) <= currentShip->length) {
					if (board[posY][posX].part == '+') {
						shipShotAt = board[posY][posX].whichShip;
						for (int i = 0; i < PLAYER_AMOUNT; i++) {
							for (int j = 0; j < playerArray[i]->fleetSize; j++) {
								if (&(playerArray[i]->ships_array[j]) == shipShotAt) {
									playerShotAt = playerArray[i];
								}
							}
						}
						for (int i = 0; i < shipShotAt->length; i++) {
							if (shipShotAt->placement[i]->x == posX && shipShotAt->placement[i]->y == posY) {
								placeShotAt = i;
							}
						}
						if (placeShotAt == shipShotAt->enginePosition) {
							shipShotAt->isEngineWorking = 0;
						}
						if (placeShotAt == shipShotAt->radarPosition) {
							shipShotAt->isRadarWorking = 0;
						}
						if (placeShotAt == shipShotAt->cannonPosition) {
							if (shipShotAt->cannonPosition == shipShotAt->enginePosition) {
								shipShotAt->isCannonWorking = 0;
								shipShotAt->isEngineWorking = 0;
							}
							else {
								shipShotAt->isCannonWorking = 0;
							}
						}
						board[posY][posX].part = 'x';
						playerShotAt->remainingShipTiles--;
					}

				}
				else {
					char text[TEXT_LENGTH] = "SHOOTING TOO FAR";
					cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << posY << " " << posX << "\": " << text << endl;
					exit(0);
				}
			}
		}
		else {
			char text[TEXT_LENGTH] = "SHIP CANNOT SHOOT";
			cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << posY << " " << posX << "\": " << text << endl;
			exit(0);
		}
		currentShip->numberOfShots--;
	}
	else {
		char text[TEXT_LENGTH] = "TOO MANY SHOOTS";
		cout << "INVALID OPERATION " << "\"" << tag << " " << iterator << " " << shipCHAR << " " << posY << " " << posX << "\": " << text << endl;
		exit(0);
	}
}
