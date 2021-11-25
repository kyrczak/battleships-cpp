#include <iostream>
#define SHIP_AMOUNT 10

using namespace std;

enum ship_category {
	DES = 2,
	CRU,
	BAT,
	CAR
};
struct player {
	ship ships_array[SHIP_AMOUNT];
};
struct ship {
	int length;
	int direction;
	int x_position;
	int y_position;
};
int main() {
	char board[21][10];
	player playerA;
	player playerB;
	return 0;
}
